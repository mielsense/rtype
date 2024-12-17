#include "network/transport/ENetServer.hpp"
#include <fmt/format.h>
#include "network/transport/ENetException.hpp"
#include "Logging.hpp"

namespace tetriq {
    ENetServer::ENetServer(ENetServer::Config &config, const std::string &server_name)
        : _config(config)
        , _server(nullptr, &enet_host_destroy)
        , _server_name(server_name)
    {
        //! do not remove, windows doesnt build without this
        if (enet_initialize() != 0) {
            std::cout << "Failed to initialize ENet" << std::endl;
        }
        if (_config.listen_address) {
            if (enet_address_set_host(&_address, _config.listen_address.value().c_str()) < 0)
                throw ENetException("Invalid ip address");
            _address.port = _config.listen_port;
            _server.reset(enet_host_create(&_address,
                                           _config.max_clients,
                                           _config.max_channels,
                                           _config.max_incoming_bandwidth,
                                           _config.max_outgoing_bandwidth));

        } else {
            _server.reset(enet_host_create(NULL,
                                           _config.max_clients,
                                           _config.max_channels,
                                           _config.max_incoming_bandwidth,
                                           _config.max_outgoing_bandwidth));

        }
        if (_server == nullptr)
            throw ENetException{fmt::format("Server {} failed to create ENet server host", _server_name).data()};

        LOG_INFO("Server {} started successfully", _server_name);
        if (_config.listen_address)
            LOG_INFO("Server {} listening on: {}:{}",
                     _server_name,
                     _config.listen_address.value(),
                     _address.port);
    }

    void ENetServer::update()
    {
        ENetEvent event;
        while (enet_host_service(_server.get(), &event, 0) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                handleNewClient(event);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                handleClientDisconnect(event);
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                handleClientReceive(event);
                break;
            case ENET_EVENT_TYPE_NONE:
            default:
                break;
            }
        }
    }

    void ENetServer::sendPacket(const APacket &packet, ITransportPeer &peer)
    {
        NetworkOStream stream{sizeof(uint64_t) + packet.getNetworkSize()};
        stream << packet.getId()
               << packet;

        ENetPacket *epacket = enet_packet_create(stream.getData(), stream.getSize(), ENET_PACKET_FLAG_RELIABLE);
        try {
            ENetTransportPeer &enet_peer = _peers.at(peer.getId());
            enet_peer_send(enet_peer.getPeer(), 0, epacket);
        } catch (const std::out_of_range &exception) {
            LOG_ERROR("Packet not sent: unknown peer id");
            return;
        }
    }

    std::queue<std::unique_ptr<TransportEvent>> &ENetServer::getEvents()
    {
        return _events;
    }

    void ENetServer::handleNewClient(const ENetEvent &event)
    {
        char ip[16];
        unsigned short port = event.peer->address.port;
        enet_address_get_host_ip(&event.peer->address, ip, sizeof(ip));

        event.peer->data = new uint64_t(_id_counter);
        _peers.try_emplace(_id_counter, event.peer, _id_counter);
        _events.emplace(std::make_unique<ConnectionEvent>(_peers.at(_id_counter)));

        _id_counter++;

        LOG_INFO("Server {} new client connected: {}:{}", _server_name, ip, port);
    }

    void ENetServer::handleClientDisconnect(const ENetEvent &event)
    {
        const uint64_t playerId = *static_cast<uint64_t *>(event.peer->data);

        delete static_cast<uint64_t *>(event.peer->data);
        event.peer->data = nullptr;

        _peers.erase(playerId);

        char ip[16];
        unsigned short port = event.peer->address.port;
        enet_address_get_host_ip(&event.peer->address, ip, sizeof(ip));

        _events.emplace(std::make_unique<DisconnectionEvent>(playerId));
        LOG_INFO("Server {} client disconnected: {}:{}", _server_name, ip, port);
    }

    void ENetServer::handleClientReceive(const ENetEvent &event)
    {
        const uint64_t id = *static_cast<uint64_t *>(event.peer->data);
        NetworkIStream is{event.packet};
        std::unique_ptr<APacket> packet;
        
        is >> packet;
        if (packet == nullptr) {
            LOG_WARNING("Server {} received invalid packet", _server_name);
            return;
        }
        _events.emplace(std::make_unique<PacketEvent>(_peers.at(id), packet));
    }

    ITransportPeer &ENetServer::getPeer(uint64_t id)
    {
        return _peers.at(id);
    }

    void ENetServer::connect(const std::string &ip, uint16_t port)
    {
        ENetAddress serverAddress;
        const int res = enet_address_set_host(&serverAddress, ip.c_str());
        if (res < 0) {
            throw ENetException("Invalid ip address");
        }
        serverAddress.port = port;

        ENetPeer *peer = enet_host_connect(_server.get(), &serverAddress, _config.max_channels, 0);
        if (peer == nullptr)
            throw ENetException("Failed to connect to server");
        peer->data = new uint64_t(_id_counter);
        _peers.try_emplace(_id_counter, peer, _id_counter);
        _id_counter++;
    }

    std::string ENetServer::getServerIp()
    {
        return _config.listen_address.value_or("");
    }

    uint16_t ENetServer::getServerPort()
    {
        return _config.listen_port;
    }
    void ENetServer::kickPeer(uint64_t id) {
        try {
            auto& peer = _peers.at(id);
            ENetPeer* enetPeer = peer.getPeer();

            //* Force immediate disconnection
            enet_peer_disconnect_now(enetPeer, 0);

            //* Clean up the peer data
            //TODO jsp ce qu'il faut cleanup


            //* Remove from peers map
            _peers.erase(id);

            //? Log the kick
            char ip[16];
            enet_address_get_host_ip(&enetPeer->address, ip, sizeof(ip));
            LOG_INFO("Server {} kicked client: {}:{}", _server_name, ip, enetPeer->address.port);
        } catch (const std::out_of_range& e) {
            LOG_ERROR("Server {} attempted to kick unknown peer: {}", _server_name, id);
        }
    }
}
