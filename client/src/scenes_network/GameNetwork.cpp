#include "scenes_network/GameNetwork.hpp"
#include "HealthComponent.hpp"
#include <utils/Data.hpp>
#include "Logging.hpp"
#include "network/packets/SetScorePacket.hpp"
#include "network/packets/ShootActionPacket.hpp"
#include "scenes_network/ISceneNetwork.hpp"

GameNetwork::GameNetwork(std::unique_ptr<tetriq::ITransportLayer> &transport, ecs::Registry &registry, Game &game) :
    ISceneNetwork(registry, transport), _game(game), _last_sent_position({0, 0}), _entityFactory(registry, game)
{
}

void GameNetwork::update(float)
{
    std::queue<Game::Action> &actions = _game.getClientActions();

    //* Process all queued actions
    while (!actions.empty()) {
        Game::Action action = actions.front();
        switch (action) {
            case Game::Action::MOVE:
                sendPlayerMovement();
                break;
            case Game::Action::SHOOT:
                sendShoot();
                break;
        }
        actions.pop();
    }

    //* Always send player movement to server
    sendPlayerMovement();
}

// Temporary, idk if this is correct
ecs::Entity GameNetwork::getPlayerEntity() const
{
    try {
        return _registry.getEntityByTag("player");
    } catch (const std::exception &e) {
        LOG_ERROR("{}", e.what());
        return ecs::NULL_ENTITY;
    }
}

void GameNetwork::sendPlayerMovement()
{
    //* Get Player
    const ecs::Entity playerEntity = getPlayerEntity(); // maybe change later
    //? If player not found, return
    if (playerEntity == ecs::NULL_ENTITY) {
        LOG_WARNING("Player entity not found");
        return;
    }

    //* Get Player Position
    const ecs::PositionComponent pos = _registry.tryGet<ecs::PositionComponent>(playerEntity);

    //* Only send if position has changed
    if (_last_sent_position.x != pos.x || _last_sent_position.y != pos.y) {
        //* Send Packet
        tetriq::Position newPos = {pos.x, pos.y};
        sendPacket(tetriq::MovementActionPacket{newPos});
        _last_sent_position = pos;
    }
}

void GameNetwork::sendShoot()
{
    //* Get Player
    ecs::Entity playerEntity = getPlayerEntity(); // maybe change later
    //? If player not found, return
    if (playerEntity == ecs::NULL_ENTITY) {
        LOG_WARNING("Player entity not found");
        return;
    }

    //* Get Player Position for the bullet origin
    ecs::PositionComponent pos = _registry.tryGet<ecs::PositionComponent>(playerEntity);
    sendPacket(tetriq::ShootActionPacket{{pos.x, pos.y}});
}

void GameNetwork::sendPacket(const tetriq::APacket &packet)
{
    _transport->sendPacket(packet, _transport->getPeer(0)); // TODO : change peer id
}

//! PACKET HANDLERS

bool GameNetwork::handleFullSynchronisationPacket(const tetriq::FullSynchronisationPacket &packet, tetriq::ITransportPeer &)
{
    //* Get fresh player entity
    ecs::Entity player{getPlayerEntity()};
    if (player == ecs::NULL_ENTITY) {
        LOG_WARNING("Player entity not found");
        return false;
    }

    //* Update player position
    _server_entities_map[packet.player_id] = player;
    _registry.set<ecs::PositionComponent>(player, {packet.player_position[0], packet.player_position[1]});
    return true;
}

bool GameNetwork::handleNewPlayerPacket(const tetriq::NewPlayerPacket &packet, tetriq::ITransportPeer &)
{
    ecs::Entity id = _entityFactory.createPlayer(packet.position);
    _server_entities_map[packet.id] = id;
    const tetriq::PlayerNamePacket player_name{DATA.playerName, 0}; //player id is ignored this way
    sendPacket(player_name);
    return true;
}

bool GameNetwork::handleNewShootPacket(const tetriq::NewShootPacket &packet, tetriq::ITransportPeer &)
{
    ecs::Entity id = _entityFactory.createBullet(packet.position, packet.velocity);
    _server_entities_map[packet.id] = id;
    return true;
}

bool GameNetwork::handleSetHealthPacket(const tetriq::SetHealthPacket &packet, tetriq::ITransportPeer &)
{
    ecs::Entity player{getPlayerEntity()};
    ecs::HealthComponent &health = _registry.tryGet<ecs::HealthComponent>(player);

    health.health = packet.health;
    health.maxHealth = packet.max_health;
    return true;
}

bool GameNetwork::handleDestroyEntityPacket(const tetriq::DestroyEntityPacket &packet, tetriq::ITransportPeer &)
{
    //* Check if entity exists
    auto it = _server_entities_map.find(packet.id);
    if (it != _server_entities_map.end()) {
        //* If entity exists, delete it then remove it from the map
        _registry.flagEntity(it->second);
        _server_entities_map.erase(it);
    }
    return true;
}

bool GameNetwork::handleEntityMovementPacket(const tetriq::EntityMovementPacket &packet, tetriq::ITransportPeer &)
{
    //* Check if entity exists
    auto it = _server_entities_map.find(packet.entity_id);
    if (it != _server_entities_map.end() && it->second != getPlayerEntity()) {
        //* If entity exists, update its position
        auto &pos = _registry.tryGet<ecs::PositionComponent>(it->second);
        pos.x = packet.position[0];
        pos.y = packet.position[1];
    }
    return true;
}

bool GameNetwork::handleStartGamePacket(const tetriq::StartGamePacket &, tetriq::ITransportPeer &)
{
    _game.startGame();
    return true;
}

bool GameNetwork::handleNewEnemyPacket(const tetriq::NewEnemyPacket &packet, tetriq::ITransportPeer &)
{
    ecs::Entity enemy = _registry.createEntity();
    std::string texture_name = "zoid";

    if (packet.type == 1) texture_name = "glider";
    else if (packet.type == 2) texture_name = "boss";
    _registry.set<ecs::PositionComponent>(enemy, ecs::PositionComponent(packet.position[0], packet.position[1]));
    _registry.set<ecs::VelocityComponent>(enemy, ecs::VelocityComponent(packet.velocity[0], packet.velocity[1]));
    _registry.set<ecs::SpriteComponent>(enemy, ecs::SpriteComponent(_game.getMapHandler().map.textures.at(texture_name).texture, {4.0f, 4.0f}));
    _registry.set<ecs::AnimationComponent>(enemy, ecs::AnimationComponent(_game.getMapHandler().map.animations.at(texture_name)));
    _registry.set<ecs::HitboxComponent>(enemy, ecs::HitboxComponent(_registry.tryGet<ecs::SpriteComponent>(enemy).sprite, {2.0f, 2.0f}));
    _registry.set<ecs::EnemyComponent>(enemy, ecs::EnemyComponent(texture_name));
    _server_entities_map[packet.id] = enemy;
    return true;
}

bool GameNetwork::handlePlayerNamePacket(const tetriq::PlayerNamePacket &player_name_packet, tetriq::ITransportPeer &)
{
    auto &player = _registry.tryGet<ecs::PlayerComponent>(_server_entities_map[player_name_packet.player_id]);
    player.setName(player_name_packet.player_name);
    return true;
}

bool GameNetwork::handleSetScorePacket(const tetriq::SetScorePacket &packet, tetriq::ITransportPeer &)
{
    auto& scoreComp = _registry.tryGet<ecs::ScoreComponent>("score");
    scoreComp.score = packet.score;
    if (static_cast<int>(packet.score) > scoreComp.highScore) {
        scoreComp.highScore = packet.score;
    }
    return true;
}

bool GameNetwork::handleEndGamePacket(const tetriq::EndGamePacket &, tetriq::ITransportPeer &)
{
    LOG_INFO("END GAME PACKET RECEIVED");

    try {
        //* Clear entities first
        //_server_entities_map.clear();
        //_registry.clear();
        LOG_INFO("ENTITIES CLEARED");

        //* Stop game state
        _game.stopGame();
        LOG_INFO("Game stopped");

        //* Queue scene change through Core
        if (_game._reconnectCallback) {
            LOG_INFO("Triggering reconnection callback");
            _game._reconnectCallback();
        }
        LOG_INFO("Callback finished");

        //_game.init(); //? je sais plus où le mettre sah
        //* Queue scene change
        //_game.fb.setView(_game.initialView);
        //_game.changeScene(SceneType::MENU); //! YA UNE BOUCLE INFINIE ICI JSP PQ
        LOG_INFO("Scene change queued");


        return true;
    } catch (const std::exception& e) {
        LOG_ERROR("Error handling end game: {}", e.what());
        return false;
    }
}
