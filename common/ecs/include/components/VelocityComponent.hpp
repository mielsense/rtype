/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** VelocityComponent
*/

#ifndef VELOCITYCOMPONENT_HPP_
#define VELOCITYCOMPONENT_HPP_

namespace ecs {
    struct VelocityComponent {
            double vx, vy; // velocity
            float sx, sy;  // speed

            explicit VelocityComponent(const double vx = 0, const double vy = 0, const float sx = 0, const float sy = 0) :
                vx(vx), vy(vy), sx(sx), sy(sy)
            {}
    };
} // namespace ecs

#endif /* !VELOCITYCOMPONENT_HPP_ */
