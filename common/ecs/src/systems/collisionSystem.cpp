/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collisionSystem
*/

#include <Registry.hpp>

#include "Entity.hpp"
#include "HitboxComponent.hpp"
#include "Logging.hpp"
#include "PositionComponent.hpp"

namespace ecs {
    ecs::Entity collisionSystem(const Entity, PositionComponent &pos, HitboxComponent &hitbox, EnemyComponent &, const std::vector<Entity> &shoots, Registry &registry)
    {
        for (const Entity &shoot : shoots) {
            const auto &shootPos = registry.tryGet<PositionComponent>(shoot);
            const auto &shootHitbox = registry.tryGet<HitboxComponent>(shoot);

            if (shootPos.x + shootHitbox.rect.width >= pos.x && shootPos.x <= pos.x + hitbox.rect.width &&
                shootPos.y + shootHitbox.rect.height >= pos.y && shootPos.y <= pos.y + hitbox.rect.height) {
                return shoot;
            }
        }
        return ecs::NULL_ENTITY;
    }
} // namespace ecs
