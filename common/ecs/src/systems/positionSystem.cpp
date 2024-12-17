/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** positionSystem
*/

#include "PositionComponent.hpp"
#include "VelocityComponent.hpp"

namespace ecs {
    void positionSystem(PositionComponent &pos, const VelocityComponent &vel, const float dt)
    {
        pos.x += vel.vx * dt;
        pos.y += vel.vy * dt;
    }
} // namespace ecs
