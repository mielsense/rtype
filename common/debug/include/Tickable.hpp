/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Tickable
*/

#pragma once
#include <chrono>

/**
 * @brief Class that can be ticked at a fixed rate.
 * FPS counter, game loop, etc. Can either be herited or used as a member.
 */
class Tickable {
    public:
        Tickable();
        explicit Tickable(uint32_t tick_rate);

        /**
         * @brief Ticks the object.
         * Will print a warning if the tick is late.
         * Will sleep until the next tick if the tick is early.
         */
        void tick();

        /**
         * @brief Set the tick rate of the object
         * @param tick_rate The tick rate in Hz (maximum ticks per second)
         */
        void setTickRate(uint32_t tick_rate);

        /**
         * @brief Reset the tickable object
         */
        void reset();
        [[nodiscard]] std::chrono::milliseconds getDeltaTime() const;

    protected:
        /**
         * @brief The next aimed tick of the object
         */
        std::chrono::steady_clock::time_point _next_tick;
        std::chrono::steady_clock::time_point _last_tick;
        std::chrono::milliseconds _delta_time;

        /**
         * @brief The tick rate of the object
         * 60 means a maximum of 60 ticks per second
         */
        uint32_t _tick_rate = 60; //@todo: should be in a config file
};
