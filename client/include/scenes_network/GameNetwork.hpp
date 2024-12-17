#pragma once

#include "display/scenes/Game.hpp"
#include "factories/EntityFactory.hpp"
#include "network/packets/EndGamePacket.hpp"
#include "network/packets/NewShootPacket.hpp"
#include "network/packets/SetHealthPacket.hpp"
#include "network/packets/SetScorePacket.hpp"
#include "network/transport/ITransportPeer.hpp"
#include "scenes_network/ISceneNetwork.hpp"


class GameNetwork: public ISceneNetwork {
    public:
        GameNetwork(std::unique_ptr<tetriq::ITransportLayer> &transport, ecs::Registry &registry, Game &game);

        //* Main update function called every frame
        void update(float deltaTime) override;

    private:
        ecs::Entity getPlayerEntity() const;

        //* SEND PACKETS
        void sendPlayerMovement();
        void sendShoot();
        void sendPacket(const tetriq::APacket &packet);

        //* Packet handlers
        bool handleFullSynchronisationPacket(const tetriq::FullSynchronisationPacket &packet, tetriq::ITransportPeer &peer) override;
        bool handleNewPlayerPacket(const tetriq::NewPlayerPacket &packet, tetriq::ITransportPeer &peer) override;
        bool handleNewShootPacket(const tetriq::NewShootPacket &packet, tetriq::ITransportPeer &peer) override;
        bool handleSetHealthPacket(const tetriq::SetHealthPacket &packet, tetriq::ITransportPeer &peer) override;
        bool handleDestroyEntityPacket(const tetriq::DestroyEntityPacket &packet, tetriq::ITransportPeer &peer) override;
        bool handleEntityMovementPacket(const tetriq::EntityMovementPacket &packet, tetriq::ITransportPeer &peer) override;
        bool handleStartGamePacket(const tetriq::StartGamePacket &packet, tetriq::ITransportPeer &peer) override;
        bool handleNewEnemyPacket(const tetriq::NewEnemyPacket &packet, tetriq::ITransportPeer &peer) override;
        bool handleSetScorePacket(const tetriq::SetScorePacket &packet, tetriq::ITransportPeer &) override;
        bool handlePlayerNamePacket(const tetriq::PlayerNamePacket &, tetriq::ITransportPeer &) override;
        bool handleEndGamePacket(const tetriq::EndGamePacket&, tetriq::ITransportPeer&) override;


        Game &_game;
        ecs::PositionComponent _last_sent_position;              //? Last position sent to the server
        std::map<ecs::Entity, ecs::Entity> _server_entities_map; //? Map to keep track of server-client entity relationships
        EntityFactory _entityFactory;                            //? Factory to create entities
};
