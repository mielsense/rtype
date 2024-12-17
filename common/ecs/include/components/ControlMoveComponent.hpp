/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ControlMoveComponent
*/

#ifndef CONTROLMOVECOMPONENT_HPP_
#define CONTROLMOVECOMPONENT_HPP_

#include <SFML/Window/Keyboard.hpp>

namespace ecs {
    struct ControlMoveComponent {
            sf::Keyboard::Key upKey;
            sf::Keyboard::Key downKey;
            sf::Keyboard::Key leftKey;
            sf::Keyboard::Key rightKey;

            explicit ControlMoveComponent(const sf::Keyboard::Key upKey = sf::Keyboard::Up,
                                          const sf::Keyboard::Key downKey = sf::Keyboard::Down,
                                          const sf::Keyboard::Key leftKey = sf::Keyboard::Left,
                                          const sf::Keyboard::Key rightKey = sf::Keyboard::Right) :
                upKey(upKey), downKey(downKey), leftKey(leftKey), rightKey(rightKey)
            {}
    };
} // namespace ecs

#endif /* !CONTROLMOVECOMPONENT_HPP_ */
