/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EntityFactory.cpp
*/

#include "factories/EntityFactory.hpp"

EntityFactory::EntityFactory(ecs::Registry &registry, Game &game) : _registry(registry), _game(game)
{}

template <typename... Components>
ecs::Entity EntityFactory::createEntity(tetriq::Position pos, Components &&...components)
{
    ecs::Entity entity = _registry.createEntity();
    _registry.set<ecs::PositionComponent>(entity, {pos[0], pos[1]});
    (_registry.set<Components>(entity, std::forward<Components>(components)), ...);
    return entity;
}

ecs::Entity EntityFactory::createPlayer(tetriq::Position pos)
{
    ecs::Entity player = createEntity(pos);
    _game.getMapHandler().addSpriteToPlayer(player);
    return player;
}

ecs::Entity EntityFactory::createBullet(tetriq::Position pos, tetriq::Velocity velocity)
{
    ecs::Entity bullet = createEntity(pos, ecs::VelocityComponent(velocity[0], velocity[1], 0.0f, 0.0f), ecs::ShootComponent());
    _game.getMapHandler().addSpriteToBullet(bullet);
    return bullet;
}
