/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PlayerComponent
*/

#ifndef PLAYERCOMPONENT_HPP
#define PLAYERCOMPONENT_HPP

#include <string>

enum PlayerColor {
    ORANGE,
    BROWN,
    GREEN,
    BLUE,
    GRAY,
    PINK

};

namespace ecs {
    struct PlayerComponent {
            std::string name;
            PlayerColor color;

            explicit PlayerComponent(std::string name = "Player 1", const PlayerColor color = ORANGE) : name(name), color(color)
            {}

            void setName(std::string input)
            {
                name = input;
            }
    };
} // namespace ecs

#endif // PLAYERCOMPONENT_HPP
