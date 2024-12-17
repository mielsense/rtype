/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** controlSystem
*/

#include <SFML/Window/Keyboard.hpp>
#include <cstdint>
#include "ControlMoveComponent.hpp"
#include "VelocityComponent.hpp"

// direction bitmasks
constexpr uint8_t UP = 0x01;
constexpr uint8_t DOWN = 0x02;
constexpr uint8_t LEFT = 0x04;
constexpr uint8_t RIGHT = 0x08;

namespace ecs {
    void controlMoveSystem(const ControlMoveComponent &ctrl, VelocityComponent &vel)
    {
        double &vx = vel.vx;
        double &vy = vel.vy;
        const float sx = vel.sx;
        const float sy = vel.sy;

        // Reset velocity
        vx = 0;
        vy = 0;

        uint8_t direction_mask = 0;

        if (sf::Keyboard::isKeyPressed(ctrl.upKey))
            direction_mask |= UP;
        if (sf::Keyboard::isKeyPressed(ctrl.downKey))
            direction_mask |= DOWN;
        if (sf::Keyboard::isKeyPressed(ctrl.leftKey))
            direction_mask |= LEFT;
        if (sf::Keyboard::isKeyPressed(ctrl.rightKey))
            direction_mask |= RIGHT;

        if (direction_mask & UP)
            vy = -sy;
        if (direction_mask & DOWN)
            vy = sy;
        if (direction_mask & LEFT)
            vx = -sx - 100.0f;
        if (direction_mask & RIGHT)
            vx = sx;

        // Reset velocity if moving in opposite directions
        if ((direction_mask & (UP | DOWN)) == (UP | DOWN))
            vy = 0;
        if ((direction_mask & (LEFT | RIGHT)) == (LEFT | RIGHT))
            vx = 0;
    }
} // namespace ecs
