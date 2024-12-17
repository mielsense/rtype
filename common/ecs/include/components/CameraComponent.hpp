/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** CameraComponent
*/

#ifndef CAMERACOMPONENT_HPP_
#define CAMERACOMPONENT_HPP_

#include <SFML/Graphics/View.hpp>

namespace ecs {
    struct CameraComponent {
            sf::View view;
            float scrollSpeed;
            bool isScrolling;
            float leftBound;
            float rightBound;
            float stopPosition;
            bool finalStop;

            CameraComponent(const float width, const float height, const float mapWidth, const float speed = 100.0f) :
                view(sf::FloatRect(0, 0, width, height)), scrollSpeed(speed), isScrolling(false), leftBound(0),
                rightBound(mapWidth - width), stopPosition(mapWidth - width), finalStop(false)
            {}
    };
} // namespace ecs

#endif // CAMERACOMPONENT_HPP_
