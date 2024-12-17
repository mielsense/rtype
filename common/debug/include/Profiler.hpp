/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Profiler
*/

#pragma once

#include <chrono>
#include <iostream>
#include <unordered_map>

class Profiler {
    public:
        void startSection(const std::string &name);

        void endSection(const std::string &name);

        void logResults() const;

    private:
        std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> sections;
        std::unordered_map<std::string, long long> timings;
};
