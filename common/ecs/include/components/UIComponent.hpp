/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef UICOMPONENT_HPP
#define UICOMPONENT_HPP

#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.hpp"

namespace ecs {
    struct UIComponent {
            sf::View view;
            bool fixed = true;

            //* Text display
            std::string mainText;                     //? Main text to display
            sf::Color textColor{sf::Color::White};    //? Text color
            int fontSize{30};                         //? Text size
            bool outline{true};                       //? Whether to draw text outline
            sf::Color outlineColor{sf::Color::Black}; //? Outline color
            float outlineThickness{2.0f};             //? Outline thickness

            //* Tooltip (for hover effects)
            std::string tooltip;
            bool isHovered = false;
            sf::FloatRect bounds;

            UIComponent(sf::Vector2f size = {1920.f, 1080.f}, const std::string &tooltipText = "") : tooltip(tooltipText)
            {
                view.setSize(size);
                view.setCenter(size.x / 2.f, size.y / 2.f);
            }
            void setTextStyle(sf::Color color, int size, bool withOutline = true, sf::Color outlineCol = sf::Color::Black,
                              float outlineThick = 2.0f)
            {
                textColor = color;
                fontSize = size;
                outline = withOutline;
                outlineColor = outlineCol;
                outlineThickness = outlineThick;
            }
    };

    // Component to link UI elements to entities they should monitor
    struct UIFollowComponent {
            ecs::Entity targetEntity;

            UIFollowComponent(ecs::Entity target) : targetEntity(target)
            {}
    };
} // namespace ecs

#endif // UICOMPONENT_HPP
