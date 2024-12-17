/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#include "HitboxComponent.hpp"
#include "PositionComponent.hpp"
#include "SpriteComponent.hpp"

namespace ecs {
    void hitboxSystem(const PositionComponent &pos, const SpriteComponent &sprite, HitboxComponent &hitbox, const bool isEditor)
    {
        const sf::FloatRect spriteBounds = sprite.sprite.getGlobalBounds();
        hitbox.rect = sf::FloatRect(pos.x - hitbox.offset.x, pos.y - hitbox.offset.y, spriteBounds.width + 2 * hitbox.offset.x,
                                    spriteBounds.height + 2 * hitbox.offset.y);

        hitbox.renderable = isEditor;
    }
} // namespace ecs
