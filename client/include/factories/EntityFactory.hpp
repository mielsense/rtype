#pragma once

#include "Registry.hpp"
#include "display/scenes/Game.hpp"
#include "network/Types.hpp"

class EntityFactory {
    public:
        EntityFactory(ecs::Registry& registry, Game& game);

        ecs::Entity createPlayer(tetriq::Position pos);
        ecs::Entity createBullet(tetriq::Position pos, tetriq::Velocity velocity);

        template<typename... Components>
        ecs::Entity createEntity(tetriq::Position pos, Components&&... components);

    private:
        ecs::Registry& _registry;
        Game& _game;
};
