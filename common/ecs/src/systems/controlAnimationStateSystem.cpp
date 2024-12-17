/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** controlAnimationStateSystem
*/

#include "AnimationComponent.hpp"
#include "ControlMoveComponent.hpp"

// direction bitmasks
constexpr uint8_t UP = 0x01;
constexpr uint8_t DOWN = 0x02;

namespace ecs {
    void controlAnimationStateSystem(const ControlMoveComponent &ctrl, AnimationComponent &anim)
    {
        uint8_t direction_mask = 0;

        if (sf::Keyboard::isKeyPressed(ctrl.upKey))
            direction_mask |= UP;
        if (sf::Keyboard::isKeyPressed(ctrl.downKey))
            direction_mask |= DOWN;

        if (direction_mask & UP)
            anim.transitionTo("UP");
        if (direction_mask & DOWN)
            anim.transitionTo("DOWN");

        if ((direction_mask & (UP | DOWN)) == (UP | DOWN) || direction_mask == 0)
            anim.transitionTo("IDLE");
    }
} // namespace ecs
