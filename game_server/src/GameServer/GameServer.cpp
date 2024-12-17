/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameServer.cpp
*/

#include "GameServer/GameServer.hpp"
#include <random>
#include "AnimationComponent.hpp"
#include "EnemyComponent.hpp"
#include "Entity.hpp"
#include "HealthComponent.hpp"
#include "HitboxComponent.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"
#include "ShootComponent.hpp"
#include "VelocityComponent.hpp"
#include "map/MapHandler.hpp"
#include "network/packets/DestroyEntityPacket.hpp"
#include "network/packets/EndGamePacket.hpp"
#include "network/packets/EntityMovementPacket.hpp"
#include "network/packets/NewEnemyPacket.hpp"
#include "network/packets/NewPlayerPacket.hpp"
#include "network/packets/NewShootPacket.hpp"
#include "network/packets/SetHealthPacket.hpp"
#include "network/packets/SetScorePacket.hpp"
#include "systems.hpp"

GameServer::GameServer(std::string server_name, uint64_t tick_rate, std::unique_ptr<tetriq::ITransportLayer> &transport,
                       uint64_t max_clients) :
    AServer(transport), _server_name(server_name), _tick_rate(tick_rate), _max_clients(max_clients), _map_handler(_registry)
{
    initRegistry();
}

void GameServer::initRegistry()
{
    try {
        //* Register components
        _registry.add<ecs::PositionComponent>();
        _registry.add<ecs::VelocityComponent>();
        _registry.add<ecs::ShootComponent>();
        _registry.add<ecs::EnemyComponent>();
        _registry.add<ecs::HitboxComponent>();
        _registry.add<ecs::PlayerComponent>();
        _registry.add<ecs::HealthComponent>();
        _registry.add<ecs::ScoreComponent>();
    } catch (const ecs::ECSException &e) {
        LOG_WARNING("Components already registered, skipping registration");
    }

    try {
        //* Create and initialize score entity
        ecs::Entity scoreEntity = _registry.createEntity();
        _registry.set<ecs::ScoreComponent>(scoreEntity, ecs::ScoreComponent{});
        _registry.tagEntity(scoreEntity, "score");
        LOG_INFO("Score entity created with tag");
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to create score entity: {}", e.what());
        throw;
    }
}
void GameServer::cleanupRegistry()
{
    //* Clear peers
    _peers.clear();

    //* clear all entities
    std::vector<ecs::Entity> entities;
    _registry.forEach<>([&](ecs::Entity entity) { entities.push_back(entity); });

    //? j'ai fait comme ça psq sinon ça crash
    for (auto entity : entities) {
        _registry.flagEntity(entity);
    }

    //*Delete flagged entities
    _registry.deleteFlaggedEntities();

    LOG_INFO("Registry cleanup complete");
}

void GameServer::onTick(float delta_time)
{
    if (getPlayerCount() == _max_clients && !_game_started) {
        _game_started = true;
        broadcastPacket(tetriq::StartGamePacket{});
        spawnEnemies(map_name);
    }

    //* If game is ending, wait for all packets to be sent before disconnecting peers
    if (_should_disconnect_peers) {
        _end_game_timer -= delta_time;
        //* Disconnect peers after delay
        if (_end_game_timer <= 0) {
            finalizeGameEnd();
            _should_disconnect_peers = false;
        }
    }
    if (_game_started)
        updateSystems(delta_time);

    _registry.deleteFlaggedEntities();
}

void GameServer::onConnection(tetriq::ConnectionEvent &event)
{
#define START_POSITION {5.0f, 5.0f}
    ecs::Entity id = _registry.createEntity();
    tetriq::NewPlayerPacket packet{id, START_POSITION};
    broadcastPacket(packet);
    _registry.set<ecs::PositionComponent>(id, START_POSITION);
    _registry.set<ecs::PlayerComponent>(id, ecs::PlayerComponent{});
    _registry.set<ecs::HitboxComponent>(id, {64, 32, {2, 2}});
    _registry.set<ecs::HealthComponent>(id, ecs::HealthComponent{});

    _transport->sendPacket(tetriq::FullSynchronisationPacket{id, START_POSITION}, event.peer);
    for (const std::pair<const unsigned long, unsigned long> &peer : _peers) {
        const auto pos = _registry.tryGet<ecs::PositionComponent>(peer.second);
        packet.id = peer.second;
        packet.position = {pos.x, pos.y};
        _transport->sendPacket(packet, event.peer);
    }
    _peers.emplace(event.peer.getId(), id);
}

void GameServer::onDisconnection(tetriq::DisconnectionEvent &event)
{
    const ecs::Entity id = _peers[event.peer_id];
    _peers.erase(event.peer_id);
    const tetriq::DestroyEntityPacket packet{id};
    _registry.flagEntity(id);
    broadcastPacket(packet);
}

bool GameServer::handleShootActionPacket(const tetriq::ShootActionPacket &packet, tetriq::ITransportPeer &)
{
    // TODO : clamp position
    const ecs::Entity bullet = _registry.createEntity();
    _registry.set<ecs::PositionComponent>(bullet, ecs::PositionComponent{packet.origin[0] + 80.0f, packet.origin[1] + 40.0f});
    _registry.set<ecs::VelocityComponent>(bullet, ecs::VelocityComponent(1000.0f, 0.0f, 0.0f, 0.0f));
    _registry.set<ecs::ShootComponent>(bullet, ecs::ShootComponent());
    _registry.set<ecs::HitboxComponent>(bullet, ecs::HitboxComponent{32, 16, {2, 2}});
    const tetriq::NewShootPacket shoot_packet{bullet, {packet.origin[0] + 100.0f, packet.origin[1] + 40.0f}, {1000.0f, 0.0f}};
    broadcastPacket(shoot_packet);
    return true;
}

bool GameServer::handleMovementActionPacket(const tetriq::MovementActionPacket &packet, tetriq::ITransportPeer &peer)
{
    const ecs::Entity player = _peers[peer.getId()];
    auto &pos = _registry.tryGet<ecs::PositionComponent>(player);
    // TODO : clamp position
    pos = {packet.new_pos[0], packet.new_pos[1]};
    broadcastPacket(tetriq::EntityMovementPacket{
        player,
        {pos.x, pos.y},
    });
    return true;
}

bool GameServer::handlePlayerNamePacket(const tetriq::PlayerNamePacket &player_name_packet, tetriq::ITransportPeer &transport_peer)
{
    for (const std::pair<const unsigned long, unsigned long> &peer : _peers) {
        if (peer.first == transport_peer.getId()) {
            continue;
        }
        tetriq::PlayerNamePacket Player_name{player_name_packet.player_name, _peers[transport_peer.getId()]};
        _transport->sendPacket(Player_name, _transport->getPeer(peer.first));
    }
    return true;
}

void GameServer::setMapName(const std::string &new_map)
{
    map_name = new_map;
}

void GameServer::updateSystems(float deltaTime)
{
    bool shouldEndGame = false;
    _registry.forEach<ecs::PositionComponent, ecs::VelocityComponent>([deltaTime](ecs::PositionComponent &pos, ecs::VelocityComponent &vel)
                                                                      { positionSystem(pos, vel, deltaTime); });

    std::vector<ecs::Entity> shoots;
    _registry.forEach<ecs::PositionComponent, ecs::HitboxComponent, ecs::ShootComponent>(
        [&](ecs::Entity id, ecs::PositionComponent &, ecs::HitboxComponent &, ecs::ShootComponent &) { shoots.push_back(id); });
    _registry.forEach<ecs::PositionComponent, ecs::HitboxComponent, ecs::EnemyComponent>(
        [&](const ecs::Entity id, ecs::PositionComponent &pos, ecs::HitboxComponent &hitbox, ecs::EnemyComponent &enemy)
        {
            ecs::Entity collision = collisionSystem(id, pos, hitbox, enemy, shoots, _registry);
            if (collision != ecs::NULL_ENTITY) {
                //* Check if the enemy is dead
                if (shootDamageSystem(id, collision, _registry)) {
                    //! score
                    auto &scoreComp = _registry.tryGet<ecs::ScoreComponent>("score");
                    scoreAddPointsSystem(scoreComp, 100);
                    //* Send the packet to all clients
                    tetriq::SetScorePacket scorePacket(scoreComp.score);
                    broadcastPacket(scorePacket);

                    //* Destroy the entities
                    _registry.flagEntity(collision);
                    _registry.flagEntity(id);
                    tetriq::DestroyEntityPacket packet{collision};
                    broadcastPacket(packet);
                    packet.id = id;
                    broadcastPacket(packet);
                } else { //* Enemy still alive, just destroy the bullet
                    _registry.flagEntity(collision);
                    tetriq::DestroyEntityPacket packet{collision};
                    broadcastPacket(packet);

                    //* Send updated health to clients
                    const auto &health = _registry.tryGet<ecs::HealthComponent>(id);
                    tetriq::SetHealthPacket healthPacket(health.health, health.maxHealth);
                    broadcastPacket(healthPacket);
                }
            }
        });
    _registry.forEach<ecs::PositionComponent, ecs::HitboxComponent, ecs::PlayerComponent, ecs::HealthComponent>(
        [&](const ecs::Entity, ecs::PositionComponent &player_pos, ecs::HitboxComponent &player_hitbox, ecs::PlayerComponent &,
            ecs::HealthComponent &health)
        {
            _registry.forEach<ecs::PositionComponent, ecs::HitboxComponent, ecs::EnemyComponent>(
                [&](const ecs::Entity id, ecs::PositionComponent &pos, ecs::HitboxComponent &hitbox, ecs::EnemyComponent &)
                {
                    if (player_pos.x + player_hitbox.rect.width >= pos.x && player_pos.x <= pos.x + hitbox.rect.width &&
                        player_pos.y + player_hitbox.rect.height >= pos.y && player_pos.y <= pos.y + hitbox.rect.height) {
                        _registry.flagEntity(id);
                        broadcastPacket(tetriq::DestroyEntityPacket{id});
                        //* Reduce health by 20%
                        health.health -= health.maxHealth / 5 - 1;
                        if (health.health <= 0) {
                            health.health = 0;
                        }
                        //* Send the packet to all clients
                        broadcastPacket(tetriq::SetHealthPacket{health.health, health.maxHealth});
                    }
                });
        });
    _registry.forEach<ecs::ShootComponent>(
        [&](const ecs::Entity id, ecs::ShootComponent &shoot)
        {
            if (shootLifetimeSystem(shoot, deltaTime)) {
                _registry.flagEntity(id);
                broadcastPacket(tetriq::DestroyEntityPacket{id});
            }
        });
    if (_registry.getEntityByTag("boss") != ecs::NULL_ENTITY) {
        _registry.forEach<ecs::EnemyComponent>(
            [&](const ecs::Entity id, ecs::EnemyComponent &)
            {
                if (id != _registry.getEntityByTag("boss")) {
                    return;
                }
                for (ecs::Entity flaggedId : _registry.getFlaggedEntities()) {
                    if (flaggedId == _registry.getEntityByTag("boss")) {
                        shouldEndGame = true;
                    }
                }
            });
    }

    if (shouldEndGame) {
        endGame();
    }
}

void GameServer::spawnEnemies(std::string &map_name)
{
    _map_handler.load(map_name, MapComponent::Enemies);
    _registry.forEach<ecs::PositionComponent, ecs::VelocityComponent, ecs::EnemyComponent>(
        [&](const ecs::Entity id, ecs::PositionComponent &pos, ecs::VelocityComponent &velocity, ecs::EnemyComponent &enemy) {
            broadcastPacket(tetriq::NewEnemyPacket{id, {pos.x, pos.y}, {velocity.vx, velocity.vy}, enemy.type});
        });
    if (_registry.getEntityByTag("boss") != ecs::NULL_ENTITY) {
        _registry.forEach<ecs::EnemyComponent>(
            [&](const ecs::Entity id, ecs::EnemyComponent &)
            {
                if (id != _registry.getEntityByTag("boss")) {
                    return;
                }
                for (ecs::Entity flaggedId : _registry.getFlaggedEntities()) {
                    if (flaggedId == _registry.getEntityByTag("boss")) {
                        endGame();
                    }
                }
            });
    }
}

void GameServer::endGame()
{
    if (!_game_started)
        return;

    //* Send end game packet to all clients
    broadcastPacket(tetriq::EndGamePacket{});

    //* Reset game state
    _game_started = false;

    //* Wait a bit to ensure packets are received
    static const float DISCONNECT_DELAY = 1.0f; //? 1 second delay
    _end_game_timer = DISCONNECT_DELAY;
    _should_disconnect_peers = true;

    LOG_INFO("Game ending - Waiting for final packets to be sent...");
}

void GameServer::finalizeGameEnd()
{
    try {
        //* Disconnect all peers first
        for (const auto &[peer_id, entity_id] : _peers) {
            _transport->kickPeer(peer_id);
        }

        //* Clean up registry
        cleanupRegistry();

        //* Reinitialize registry
        initRegistry();

        //* Reset game state
        _game_started = false;

        LOG_INFO("Game ended - Server reset complete");
    } catch (const std::exception &e) {
        LOG_ERROR("Error during game end: {}", e.what());
    }
}

uint64_t GameServer::getPlayerCount() const
{
    return _peers.size();
}

uint64_t GameServer::getMaxClients() const
{
    return _max_clients;
}

uint64_t GameServer::getTickRate() const
{
    return _tick_rate;
}

const std::string &GameServer::getServerName() const
{
    return _server_name;
}

uint64_t GameServer::getLevel() const
{
    return 0; // todo
}

std::string GameServer::getServerIp() const
{
    return _transport->getServerIp();
}

uint16_t GameServer::getServerPort() const
{
    return _transport->getServerPort();
}

void GameServer::broadcastPacket(const tetriq::APacket &packet)
{
    for (const std::pair<const unsigned long, unsigned long> &peer : _peers) {
        _transport->sendPacket(packet, _transport->getPeer(peer.first));
    }
}
