/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ControlShootComponent
*/

#ifndef CONTROLSHOOTCOMPONENT_HPP_
#define CONTROLSHOOTCOMPONENT_HPP_

#include <SFML/Window/Keyboard.hpp>

namespace ecs {
    struct ControlShootComponent {
            sf::Keyboard::Key shootKey;

            float cooldown;
            float timer;
            bool isShooting;

            explicit ControlShootComponent(sf::Keyboard::Key key = sf::Keyboard::Space, float cd = 0.15f) :
                shootKey(key), cooldown(cd), timer(0.0f), isShooting(false)
            {}
    };
} // namespace ecs

#endif /* !CONTROLSHOOTCOMPONENT_HPP_ */
