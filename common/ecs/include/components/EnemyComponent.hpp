/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EnemyComponent
*/

#ifndef ENEMYCOMPONENT_HPP
#define ENEMYCOMPONENT_HPP

#include <cstdint>
#include <string>
namespace ecs {
    struct EnemyComponent {
            uint64_t type = 0;
            EnemyComponent(const std::string &name)
                {
                    if (name == "glider")
                        type = 1;
                    else if (name == "boss")
                        type = 2;
                }
    };
} // namespace ecs

#endif // ENEMYCOMPONENT_HPP
