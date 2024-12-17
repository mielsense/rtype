/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** shootSystem
*/

#include <SFML/Window/Keyboard.hpp>
#include "ControlShootComponent.hpp"

namespace ecs {
    bool controlShootSystem(ControlShootComponent &shoot, const float dt)
    {
        shoot.timer += dt;
        if (sf::Keyboard::isKeyPressed(shoot.shootKey) && shoot.timer >= shoot.cooldown) {
            shoot.timer = 0.0f;
            shoot.isShooting = true;
        } else {
            shoot.isShooting = false;
        }
        return shoot.isShooting;
    }
} // namespace ecs
