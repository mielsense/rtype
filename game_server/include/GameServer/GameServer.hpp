/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameServer
*/

#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include "map/MapHandler.hpp"
#include "network/AServer.hpp"
#include "network/packets/MovementActionPacket.hpp"
#include "network/packets/ShootActionPacket.hpp"
#include "network/transport/ITransportPeer.hpp"

/**
 * Server that will be used to communicate with the clients (players)
 */
class GameServer: public tetriq::AServer {
    public:
        GameServer(std::string server_name, uint64_t tick_rate, std::unique_ptr<tetriq::ITransportLayer> &transport, uint64_t max_clients);

        void onTick(float delta_time) override;
        void onConnection(tetriq::ConnectionEvent &event) override;
        void onDisconnection(tetriq::DisconnectionEvent &event) override;

        bool handleShootActionPacket(const tetriq::ShootActionPacket &packet, tetriq::ITransportPeer &peer) override;
        bool handleMovementActionPacket(const tetriq::MovementActionPacket &packet, tetriq::ITransportPeer &peer) override;
        bool handlePlayerNamePacket(const tetriq::PlayerNamePacket &, tetriq::ITransportPeer &) override;

        void setMapName(const std::string &map_name);

        void updateSystems(float deltaTime);
        void spawnEnemies(std::string &map_name);

        void endGame();
        void initRegistry();
        void cleanupRegistry();


        // Getters
        uint64_t getPlayerCount() const;
        uint64_t getMaxClients() const;
        uint64_t getTickRate() const;
        const std::string &getServerName() const;
        uint64_t getLevel() const;
        std::string getServerIp() const;
        uint16_t getServerPort() const;

    private:
        void broadcastPacket(const tetriq::APacket &packet);

        std::unordered_map<uint64_t, ecs::Entity> _peers;
        std::string _server_name;
        uint64_t _tick_rate;
        uint64_t _max_clients;
        ecs::Registry _registry;
        bool _game_started = false;
        MapHandler _map_handler;
        std::string map_name = "client/assets/maps/map-1.json";

        float _end_game_timer{0.0f};
        bool _should_disconnect_peers{false};
        void finalizeGameEnd();
};
