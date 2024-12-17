/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MasterServer
*/

#pragma once

#include "network/AServer.hpp"
#include "network/transport/ITransportPeer.hpp"

#include <unordered_map>
#include <chrono>

class MasterServer {
    public:
        struct ConnectedGameServer {
                std::string name;
                std::uint64_t max_players;
                std::uint64_t current_players;
                std::chrono::steady_clock::time_point last_answer;
                uint64_t peer_id;
                std::string ip;
                uint16_t port;
        };
        using ServerList = std::unordered_map<uint64_t, ConnectedGameServer>;

        const ServerList &getServerList() const;
        ServerList &getServerList();
        void updateServer(uint64_t peer_id, const ConnectedGameServer &&server);
    private:
         ServerList _game_servers;
};

class MasterServerForPlayers final: public tetriq::AServer {
    public:
        MasterServerForPlayers(MasterServer &master, std::unique_ptr<tetriq::ITransportLayer> &transport);
        bool handleServerListRequestPacket(const tetriq::ServerListRequestPacket&, tetriq::ITransportPeer&) override;
    private:
        MasterServer &master;
};

class MasterServerForServers final: public tetriq::AServer {
    public:
        MasterServerForServers(MasterServer &master, std::unique_ptr<tetriq::ITransportLayer> &transport);

        void removeDisconnectedServers();
        void sendQueryPackets() const;

        void onTick(float delta_time) override;
        void onConnection(tetriq::ConnectionEvent &event) override;
        void onDisconnection(tetriq::DisconnectionEvent &event) override;

        // Packet between servers (game servers and master server)
        bool handlePingPacket(const tetriq::PingPacket &, tetriq::ITransportPeer &peer) override;
        bool handleQueryResponsePacket(const tetriq::QueryResponsePacket &, tetriq::ITransportPeer &) override;
    private:
        MasterServer &master;
};
