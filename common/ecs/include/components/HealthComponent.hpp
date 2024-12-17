/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef HEALTHCOMPONENT_HPP
#define HEALTHCOMPONENT_HPP

#pragma once

namespace ecs {
    struct HealthComponent {
            float health;
            float maxHealth;

            HealthComponent(float maxHealth, float health) : health(health), maxHealth(maxHealth)
            {}

            explicit HealthComponent(float maxHealth = 100.0f) : health(maxHealth), maxHealth(maxHealth)
            {}
    };
} // namespace ecs

#endif // HEALTHCOMPONENT_HPP
