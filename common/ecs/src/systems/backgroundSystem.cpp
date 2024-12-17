/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** backgroundSystem
*/

#include <SFML/Graphics/View.hpp>
#include "BackgroundComponent.hpp"

namespace ecs {
    void backgroundSystem(BackgroundComponent &bg, const float dt, const sf::View &view)
    {
        if (!bg.isScrolling)
            return;

        const sf::FloatRect viewBounds(view.getCenter() - view.getSize() / 2.f, view.getSize());

        for (auto &element : bg.elements) {
            if (element.isRandom) {
                // Move random elements
                sf::Vector2f pos = element.sprite.getPosition();
                pos.x -= element.scrollSpeed * dt;

                // Wrap around if off-screen, but only if not past the map end
                if (pos.x + element.sprite.getGlobalBounds().width < viewBounds.left) {
                    if (viewBounds.left + viewBounds.width < bg.mapWidth) {
                        pos.x = viewBounds.left + viewBounds.width;
                        pos.y = std::uniform_real_distribution<float>(0, viewBounds.height)(bg.rng);
                    } else {
                        // If past map end, stop spawning new elements
                        pos.x = bg.mapWidth;
                    }
                }

                element.sprite.setPosition(pos);
            } else {
                // Scroll regular elements
                const float offset = std::fmod(view.getCenter().x * (element.scrollSpeed / 100.f), element.sprite.getGlobalBounds().width);
                element.sprite.setPosition(-offset, 0);
            }
        }
    }

    void drawBackgroundSystem(sf::RenderTarget &target, BackgroundComponent &bg, const sf::View &view)
    {
        const sf::FloatRect viewBounds(view.getCenter() - view.getSize() / 2.f, view.getSize());

        for (const auto &element : bg.elements) {
            if (!element.isRandom || element.sprite.getGlobalBounds().intersects(viewBounds)) {
                target.draw(element.sprite);
            }
        }
    }
} // namespace ecs
