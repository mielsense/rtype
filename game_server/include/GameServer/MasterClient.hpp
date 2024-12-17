#pragma once

#include "GameServer/GameServer.hpp"
#include "network/AServer.hpp"
#include "network/transport/ITransportLayer.hpp"

/**
 * Client that will be used to communicate with the master server
 */
class MasterClient final: public tetriq::AServer {
    public:
        MasterClient(GameServer &server, std::unique_ptr<tetriq::ITransportLayer> &transport);

        void onTick(float delta_time) override;
        void onConnection(tetriq::ConnectionEvent &event) override;

        // Packet between game server and master server
        bool handleQueryRequestPacket(const tetriq::QueryRequestPacket &, tetriq::ITransportPeer &peer) override;

    private:
        GameServer &server;
};
