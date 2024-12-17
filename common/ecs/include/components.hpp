/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** components
*/

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "AnimationComponent.hpp"
#include "BackgroundComponent.hpp"
#include "CameraComponent.hpp"
#include "ControlMoveComponent.hpp"
#include "ControlShootComponent.hpp"
#include "EnemyComponent.hpp"
#include "HealthComponent.hpp"
#include "HitboxComponent.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"
#include "ShootComponent.hpp"
#include "SpriteComponent.hpp"
#include "UIComponent.hpp"
#include "VelocityComponent.hpp"
#include "ScoreComponent.hpp"

namespace ecs {
    constexpr std::size_t MAX_COMPONENTS = 64; // multiple of 32 for memory alignment
} // namespace ecs

#endif // COMPONENTS_HPP
