/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** BackgroundComponent
*/

#ifndef BACKGROUNDCOMPONENT_HPP_
#define BACKGROUNDCOMPONENT_HPP_

#include <SFML/Graphics.hpp>
#include <list>
#include <random>

namespace ecs {
    struct BackgroundElement {
            sf::Sprite sprite;
            float scrollSpeed;
            bool isRandom;
            sf::Vector2f initialPosition;
    };

    struct BackgroundComponent {
            std::list<BackgroundElement> elements;

            float mapWidth;
            bool isScrolling;
            std::mt19937 rng;

            explicit BackgroundComponent(const float mapWidth) : mapWidth(mapWidth), isScrolling(true)
            {
                rng.seed(std::random_device()());
            }

            void addRegularElement(sf::Texture &texture, const float scrollSpeed, const sf::Vector2u &windowSize)
            {
                BackgroundElement element;
                element.scrollSpeed = scrollSpeed;
                element.isRandom = false;

                const float scaleY = static_cast<float>(windowSize.y) / texture.getSize().y;
                const float scaleX = scaleY;

                element.sprite.setTexture(texture);
                element.sprite.setScale(scaleX, scaleY);

                texture.setRepeated(true);
                element.sprite.setTextureRect(sf::IntRect(0, 0, std::ceil((mapWidth + windowSize.x) / scaleX), texture.getSize().y));

                elements.push_back(element);
            }

            void addRandomElement(const sf::Texture &texture, const float scrollSpeed, const sf::Vector2u &windowSize)
            {
                BackgroundElement element;
                element.scrollSpeed = scrollSpeed;
                element.isRandom = true;

                const float scale = std::uniform_real_distribution(2.0f, 5.0f)(rng);
                element.sprite.setTexture(texture);
                element.sprite.setScale(scale, scale);

                float x = std::uniform_real_distribution<float>(0, mapWidth)(rng);
                float y = std::uniform_real_distribution<float>(0, windowSize.y)(rng);
                element.initialPosition = {x, y};
                element.sprite.setPosition(element.initialPosition);

                elements.push_back(element);
            }
    };
} // namespace ecs

#endif // BACKGROUNDCOMPONENT_HPP_
