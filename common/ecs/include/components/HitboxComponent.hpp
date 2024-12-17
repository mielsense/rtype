/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** HitboxComponent
*/

#ifndef HITBOXCOMPONENT_HPP
#define HITBOXCOMPONENT_HPP

#include <SFML/Graphics.hpp>

namespace ecs {
    struct HitboxComponent {
            sf::FloatRect rect;
            sf::Vector2f offset;
            bool renderable = false;

            // Constructor for a simple rectangular hitbox
            HitboxComponent(const float width, const float height, const sf::Vector2f offset = {0.0f, 0.0f}) :
                rect(0, 0, width, height), offset(offset)
            {}

            // Constructor using sf::FloatRect
            explicit HitboxComponent(const sf::FloatRect &rectangle, const sf::Vector2f offset = {0.0f, 0.0f}) :
                rect(rectangle), offset(offset)
            {}

            // Constructor that takes an sf::Sprite and an offset
            explicit HitboxComponent(const sf::Sprite &sprite, const sf::Vector2f offset = {0.0f, 0.0f}) :
                rect(sprite.getGlobalBounds()), offset(offset)
            {}
    };
} // namespace ecs

#endif // HITBOXCOMPONENT_HPP
