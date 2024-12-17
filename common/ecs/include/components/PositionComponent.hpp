/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PositionComponent
*/

#ifndef POSITIONCOMPONENT_HPP_
#define POSITIONCOMPONENT_HPP_

namespace ecs {
    struct PositionComponent {
            double x, y;

            PositionComponent(const double x = 0, const double y = 0) : x(x), y(y)
            {}
    };
} // namespace ecs

#endif /* !POSITIONCOMPONENT_HPP_ */
