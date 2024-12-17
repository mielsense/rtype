/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** cleanupSystem
*/

#ifndef CLEANUP_SYSTEM_HPP
#define CLEANUP_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include "Logging.hpp"
#include "PositionComponent.hpp"
#include "Registry.hpp"

#define MARGIN 100.0f

namespace ecs {

    void cleanupSystem(Registry &registry, const sf::View &cameraView)
    {
        const sf::Vector2f cameraCenter = cameraView.getCenter();
        const sf::Vector2f cameraSize = cameraView.getSize();
        const float leftBound = cameraCenter.x - cameraSize.x / 2 - MARGIN;

        std::vector<Entity> entitiesToRemove;

        registry.forEach<PositionComponent>(
            [&](const Entity id, const PositionComponent &pos)
            {
                if (pos.x + MARGIN < leftBound) {
                    entitiesToRemove.push_back(id);
                }
            });

        for (auto &entity : entitiesToRemove) {
            registry.flagEntity(entity);
        }

        if (!entitiesToRemove.empty()) {
            LOG_DEBUG("Cleaned up {} entities", entitiesToRemove.size());
        }
    }

} // namespace ecs

#endif // CLEANUP_SYSTEM_HPP
