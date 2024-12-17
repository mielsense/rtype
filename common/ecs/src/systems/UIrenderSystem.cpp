/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#include <SFML/Graphics.hpp>
#include "PositionComponent.hpp"
#include "SpriteComponent.hpp"
#include "UIComponent.hpp"

namespace ecs {
    void UIrenderSystem(sf::RenderTexture &fb, PositionComponent &pos, SpriteComponent &sprite, UIComponent &ui,
                        const sf::Font &font)
    {
        // Store current view
        sf::View currentView = fb.getView();
        fb.setView(ui.view);

        // Draw UI element
        sprite.sprite.setPosition(pos.x, pos.y);
        fb.draw(sprite.sprite);


        // Main text rendering
        if (!ui.mainText.empty()) {
            sf::Text text;
            text.setFont(font);
            text.setString(ui.mainText);
            text.setCharacterSize(ui.fontSize);
            text.setFillColor(ui.textColor);

            if (ui.outline) {
                text.setOutlineColor(ui.outlineColor);
                text.setOutlineThickness(ui.outlineThickness);
            }

            text.setPosition(pos.x, pos.y);
            fb.draw(text);
        }
        // Draw tooltip if hovered
        if (ui.isHovered && !ui.tooltip.empty()) {
            sf::Text text;
            text.setFont(font);
            text.setString(ui.tooltip);
            text.setCharacterSize(25);
            text.setFillColor(sf::Color::White);

            // Position tooltip above the element
            float tooltipX = pos.x;
            float tooltipY = pos.y - text.getGlobalBounds().height - 5.f;

            // Keep tooltip on screen
            if (tooltipY < 0) {
                tooltipY = pos.y + sprite.sprite.getGlobalBounds().height + 5.f;
            }

            text.setPosition(tooltipX, tooltipY);
            fb.draw(text);
        }

        fb.setView(currentView);
    }
} // namespace ecs
