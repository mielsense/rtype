/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shootDamageSystem
*/

#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    bool shootDamageSystem(Entity targetEntity, Entity shootEntity, Registry &registry) {
        //* Get the health and shoot components of the target and shoot entities
        auto &health = registry.tryGet<HealthComponent>(targetEntity);
        const auto &shoot = registry.tryGet<ShootComponent>(shootEntity);

        //* Reduce the health of the target entity by the damage of the shoot entity
        health.health -= shoot.damage;

        //* Return true if the target entity has no health left
        return health.health <= 0;
    }
}