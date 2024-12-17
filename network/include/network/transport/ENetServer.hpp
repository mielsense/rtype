#pragma once

#include <enet/enet.h>
#include <optional>
#include <string>
#include <unordered_map>
#include "network/transport/ENetTransportPeer.hpp"
#include "network/transport/ITransportLayer.hpp"
#include "network/transport/ITransportPeer.hpp"
#include "network/transport/TransportEvent.hpp"

namespace tetriq {
    class ENetServer: public ITransportLayer {
        public:
            class Config {
                public:
                    std::optional<std::string> listen_address;
                    uint16_t listen_port;
                    uint64_t max_clients;
                    uint64_t max_channels;
                    uint64_t max_incoming_bandwidth = 0;
                    uint64_t max_outgoing_bandwidth = 0;
            };

            ENetServer(Config &config, const std::string &server_name);
            ~ENetServer() override = default;

            void sendPacket(const APacket &packet, ITransportPeer &peer) override;
            void update() override;
            std::queue<std::unique_ptr<TransportEvent>> &getEvents() override;
            ITransportPeer &getPeer(uint64_t id) override;
            void connect(const std::string &ip, uint16_t port) override;
            void kickPeer(uint64_t id) override;
            std::string getServerIp() override;
            uint16_t getServerPort() override;

        private:
            void handleNewClient(const ENetEvent &event);
            void handleClientDisconnect(const ENetEvent &event);
            void handleClientReceive(const ENetEvent &event);

            using _Server = std::unique_ptr<ENetHost, decltype(&enet_host_destroy)>;

            Config _config;
            _Server _server;
            ENetAddress _address{};
            std::string _server_name;
            std::unordered_map<uint64_t, ENetTransportPeer> _peers;
            std::queue<std::unique_ptr<TransportEvent>> _events;
            uint64_t _id_counter{0};
    };
} // namespace tetriq
