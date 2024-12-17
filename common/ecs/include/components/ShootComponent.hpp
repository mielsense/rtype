/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShootComponent
*/

#ifndef SHOOTCOMPONENT_HPP
#define SHOOTCOMPONENT_HPP

namespace ecs {
    static constexpr float DEFAULT_DAMAGE = 50.0f;
    struct ShootComponent {
            float lifetime;
            float damage;

            explicit ShootComponent(const float lifetime = 1.4f, const float dmg = DEFAULT_DAMAGE) : lifetime(lifetime), damage(dmg)
            {}
    };
} // namespace ecs

#endif // SHOOTCOMPONENT_HPP
