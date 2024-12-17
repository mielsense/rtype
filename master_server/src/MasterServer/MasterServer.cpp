/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MasterServer.cpp
*/

#include "MasterServer/MasterServer.hpp"
#include "Logging.hpp"
#include "network/transport/TransportEvent.hpp"

const MasterServer::ServerList &MasterServer::getServerList() const
{
    return _game_servers;
}

MasterServer::ServerList &MasterServer::getServerList()
{
    return _game_servers;
}

void MasterServer::updateServer(uint64_t peer_id, const ConnectedGameServer &&server)
{
    _game_servers[peer_id] = server;
}

MasterServerForServers::MasterServerForServers(MasterServer &master,
                                               std::unique_ptr<tetriq::ITransportLayer> &transport) :
    AServer(transport), master(master)
{
}

void MasterServerForServers::sendQueryPackets() const
{
    auto now = std::chrono::system_clock::now();
    static auto last = now;
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();

    // hardcoded 5 seconds
    if (elapsed > 5000) {
        last = now;
        tetriq::QueryRequestPacket query_packet;

        for (auto &server : master.getServerList()) {
            _transport->sendPacket(query_packet, _transport->getPeer(server.second.peer_id));
        }
    }
}

void MasterServerForServers::removeDisconnectedServers()
{
    MasterServer::ServerList &server_list = master.getServerList();
    const auto now = std::chrono::steady_clock::now();
    for (auto it = server_list.begin(); it != server_list.end();) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - it->second.last_answer).count();
        // hardcoded 10 seconds
        if (elapsed > 10) {
            it = server_list.erase(it);
        } else {
            ++it;
        }
    }
}

bool MasterServerForServers::handlePingPacket(const tetriq::PingPacket &, tetriq::ITransportPeer &peer)
{
    tetriq::PingPacket answer{time(nullptr)};
    _transport->sendPacket(answer, peer);
    return true;
}

bool MasterServerForServers::handleQueryResponsePacket(const tetriq::QueryResponsePacket &query_response_packet,
                                                       tetriq::ITransportPeer &peer)
{
    //LOG_INFO("[MASTER_CLIENT] Received query response packet:\n"
    //         "Player count: {}\n"
    //         "Max players: {}\n"
    //         "Tick rate: {}\n"
    //         "Server name: {}\n"
    //         "Current level: {}\n"
    //         "Server IP: {}\n"
    //         "Server port: {}",
    //         query_response_packet.player_count, query_response_packet.max_players, query_response_packet.tick_rate,
    //         query_response_packet.server_name, query_response_packet.current_level, query_response_packet.server_ip,
    //         query_response_packet.server_port);

    master.updateServer(peer.getId(),
                        {query_response_packet.server_name, query_response_packet.max_players,
                         query_response_packet.player_count, std::chrono::steady_clock::now(), peer.getId(),
                         query_response_packet.server_ip, query_response_packet.server_port});
    return true;
}

void MasterServerForServers::onTick(float)
{
    removeDisconnectedServers();
    sendQueryPackets();
}

void MasterServerForServers::onConnection(tetriq::ConnectionEvent &event)
{
    tetriq::QueryRequestPacket query_packet;
    _transport->sendPacket(query_packet, event.peer);
}

void MasterServerForServers::onDisconnection(tetriq::DisconnectionEvent &event)
{
    MasterServer::ServerList &server_list = master.getServerList();
    server_list.erase(event.peer_id);
}

MasterServerForPlayers::MasterServerForPlayers(MasterServer &master,
                                               std::unique_ptr<tetriq::ITransportLayer> &transport) :
    AServer(transport), master(master)
{
}

bool MasterServerForPlayers::handleServerListRequestPacket(
    const tetriq::ServerListRequestPacket &, tetriq::ITransportPeer &transport_peer)
{
    std::vector<tetriq::ServerInfo> servers;
    for (auto &server : master.getServerList()) {
        servers.emplace_back(server.second.name, server.second.ip, server.second.port,
                                                server.second.current_players, server.second.max_players);
    }
    const tetriq::ServerListResponsePacket response_packet{servers};
    _transport->sendPacket(response_packet, transport_peer);
    return true;
}

