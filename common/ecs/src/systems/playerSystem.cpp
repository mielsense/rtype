/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** playerSystem
*/

#ifndef PLAYERSYSTEM_HPP_
#define PLAYERSYSTEM_HPP_

#include "CameraComponent.hpp"
#include "HitboxComponent.hpp"
#include "PositionComponent.hpp"
#include "VelocityComponent.hpp"

#include <algorithm>

namespace ecs {
    void playerSystem(PositionComponent &pos, VelocityComponent &, const CameraComponent &camera, const float dt,
                      const HitboxComponent &hitbox)
    {
        // Move player with camera only if the camera is still scrolling
        if (camera.isScrolling && !camera.finalStop) {
            pos.x += camera.scrollSpeed * dt;
        }

        // Constrain player within camera bounds
        const float leftBound = camera.view.getCenter().x - camera.view.getSize().x / 2 + 50;
        const float rightBound = camera.view.getCenter().x + camera.view.getSize().x / 2 - 50;
        const float topBound = camera.view.getCenter().y - camera.view.getSize().y / 2 + 50;
        const float bottomBound = camera.view.getCenter().y + camera.view.getSize().y / 2 - 50;

        pos.x = std::max(leftBound, std::min(rightBound - hitbox.rect.width, static_cast<float>(pos.x)));
        pos.y = std::max(topBound, std::min(bottomBound - hitbox.rect.height, static_cast<float>(pos.y)));
    }
} // namespace ecs

#endif // PLAYERSYSTEM_HPP_
