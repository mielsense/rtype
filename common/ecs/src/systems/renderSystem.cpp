/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** renderSystem
*/

#include <SFML/Graphics.hpp>
#include "HitboxComponent.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"
#include "SpriteComponent.hpp"

namespace ecs {
    void renderSystem(sf::RenderTexture &fb, const PositionComponent &pos, SpriteComponent &sprite, const HitboxComponent &hitbox,
                      const PlayerComponent *player, const sf::View &cameraView, const sf::Font &font)
    {
        const sf::Vector2f cameraCenter = cameraView.getCenter();
        const sf::Vector2f cameraSize = cameraView.getSize();
        const sf::FloatRect cameraBounds(cameraCenter.x - cameraSize.x / 2, cameraCenter.y - cameraSize.y / 2, cameraSize.x, cameraSize.y);

        if (cameraBounds.intersects(
                sf::FloatRect(pos.x, pos.y, sprite.sprite.getGlobalBounds().width, sprite.sprite.getGlobalBounds().height))) {

            sprite.sprite.setPosition(pos.x, pos.y);
            fb.draw(sprite.sprite);

            if (hitbox.renderable) {
                sf::RectangleShape hitboxShape;
                hitboxShape.setPosition(hitbox.rect.left, hitbox.rect.top);
                hitboxShape.setSize(sf::Vector2f(hitbox.rect.width, hitbox.rect.height));
                hitboxShape.setFillColor(sf::Color::Transparent);
                hitboxShape.setOutlineColor(sf::Color::Magenta);
                hitboxShape.setOutlineThickness(2.0f);
                fb.draw(hitboxShape);
            }

            if (player != nullptr) {
                sf::Text nameText;
                nameText.setFont(font);
                nameText.setString(player->name);
                nameText.setCharacterSize(25);

                sf::Color textColor;

                nameText.setFillColor(sf::Color::White);

                nameText.setOutlineColor(sf::Color::Black);
                nameText.setOutlineThickness(1.0f);

                sf::FloatRect textBounds = nameText.getLocalBounds();
                float textX = pos.x + (sprite.sprite.getGlobalBounds().width - textBounds.width) / 2;
                float textY = pos.y - textBounds.height - 5;
                nameText.setPosition(textX, textY);

                fb.draw(nameText);
            }
        }
    }
} // namespace ecs
