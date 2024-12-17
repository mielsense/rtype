/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#include "utils/Waitable.hpp"

Waitable::Waitable(std::chrono::seconds dur, std::function<void()> cb) : duration(dur), callback(cb), active(false)
{}

void Waitable::start()
{
    active = true;
    next = std::chrono::steady_clock::now() + duration;
}

void Waitable::stop()
{
    active = false;
}

void Waitable::update()
{
    if (!active)
        return;

    auto now = std::chrono::steady_clock::now();
    if (now >= next) {
        callback();
        next = now + duration;
    }
}
