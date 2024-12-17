/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef WAITABLE_HPP
#define WAITABLE_HPP

#pragma once

#include <chrono>
#include <functional>

class Waitable {
    private:
        std::chrono::steady_clock::duration duration; // duration between each call
        std::chrono::steady_clock::time_point next;   // next time to call the callback

        std::function<void()> callback;
        bool active;

    public:
        Waitable(std::chrono::seconds dur, std::function<void()> cb);
        ~Waitable() = default;

        // class methods

        void start();
        void stop();
        void update();

        // getters and setters

        bool isActive() const
        {
            return active;
        }
};

#endif // WAITABLE_HPP
