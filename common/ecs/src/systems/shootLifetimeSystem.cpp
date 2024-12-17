/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shootDurationSystem
*/

#include <ShootComponent.hpp>
#include <Entity.hpp>

namespace ecs {
    bool shootLifetimeSystem(ShootComponent &shoot, const float dt) {
        shoot.lifetime -= dt;
        if (shoot.lifetime < 0)
            return true;
        return false;
    }
}