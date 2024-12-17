/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SpriteComponent
*/

#ifndef SPRITECOMPONENT_HPP_
#define SPRITECOMPONENT_HPP_

#include <SFML/Graphics/Sprite.hpp>

namespace ecs {
    struct SpriteComponent {
            sf::Sprite sprite;

            SpriteComponent() = default;

            explicit SpriteComponent(const sf::Texture &texture, const sf::Vector2f scale = {1.0f, 1.0f})
            {
                sprite.setTexture(texture);
                sprite.scale(scale);
            }
    };
} // namespace ecs

#endif /* !SPRITECOMPONENT_HPP_ */
