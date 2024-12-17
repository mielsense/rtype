/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Tickable.cpp
*/

#include "Tickable.hpp"

#include <Logging.hpp>
#include <thread>

Tickable::Tickable() : _delta_time()
{
    LOG_DEBUG("Tickable created: tick_rate={}hz", _tick_rate);
    reset();
}

Tickable::Tickable(const uint32_t tick_rate) :
     _delta_time(), _tick_rate(tick_rate)
{
    LOG_DEBUG("Tickable created: tick_rate={}hz", _tick_rate);
    reset();
}

void Tickable::tick()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    _delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - _last_tick);
    _last_tick = now;

    if (now > _next_tick) {
        LOG_WARNING("main loop is running behind : {}ms",
                    std::chrono::duration_cast<std::chrono::milliseconds>(now - _next_tick).count());
        reset();
    } else {
        std::this_thread::sleep_until(_next_tick);
        _next_tick += std::chrono::nanoseconds(1'000'000'000 / _tick_rate);
    }
}

void Tickable::setTickRate(const uint32_t tick_rate)
{
    _tick_rate = tick_rate;
    reset();
    LOG_DEBUG("Tick rate set to {}hz", _tick_rate);
}

void Tickable::reset()
{
    _next_tick = std::chrono::steady_clock::now() + std::chrono::nanoseconds(1'000'000'000 / _tick_rate);
}

std::chrono::milliseconds Tickable::getDeltaTime() const
{
    return _delta_time;
}