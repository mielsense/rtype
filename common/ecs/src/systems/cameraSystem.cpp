/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** cameraSystem
*/

#ifndef CAMERASYSTEM_HPP_
#define CAMERASYSTEM_HPP_

#include <algorithm>
#include "CameraComponent.hpp"

namespace ecs {
    void cameraSystem(CameraComponent &camera, const float dt)
    {
        if (camera.isScrolling && !camera.finalStop) {
            float newX = camera.view.getCenter().x + camera.scrollSpeed * dt;

            // Clamp the position between the start of the map and the stop position
            newX = std::clamp(newX, camera.view.getSize().x / 2, camera.stopPosition);

            camera.view.setCenter(newX, camera.view.getCenter().y);
            camera.leftBound = camera.view.getCenter().x - camera.view.getSize().x / 2;

            if (newX >= camera.stopPosition) {
                camera.isScrolling = false;
                camera.finalStop = true;
            }
        }
    }
} // namespace ecs

#endif // CAMERASYSTEM_HPP_
