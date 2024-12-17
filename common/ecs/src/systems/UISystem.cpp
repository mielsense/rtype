/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#include "PositionComponent.hpp"
#include "SpriteComponent.hpp"
#include "UIComponent.hpp"

namespace ecs {
    void UISystem(UIComponent &ui, PositionComponent &pos, SpriteComponent &sprite, const sf::Vector2f &mousePos)
    {
        sf::FloatRect spriteBounds = sprite.sprite.getGlobalBounds();
        ui.bounds = sf::FloatRect(pos.x, pos.y, spriteBounds.width, spriteBounds.height);
        ui.isHovered = ui.bounds.contains(mousePos);
    }
} // namespace ecs
