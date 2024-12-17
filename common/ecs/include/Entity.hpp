/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity
*/

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>
#include <limits>

namespace ecs {
    /// @brief Defines the type used for representing an entity.
    using Entity = uint64_t;

    /// @brief Constant for an invalid entity or a null entity.
    static constexpr Entity NULL_ENTITY = std::numeric_limits<Entity>::max();

    /**
     * @brief Maximum number of entities allowed in the system.
     *
     * This is set to NULL_ENTITY by default, which represents no limit.
     * If the limit is reached, the program will terminate.
     */
    constexpr std::size_t MAX_ENTITIES = NULL_ENTITY;
} // namespace ecs

#endif // ENTITY_HPP