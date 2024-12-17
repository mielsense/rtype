/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Profiler
*/

#include "Profiler.hpp"

void Profiler::startSection(const std::string &name)
{
    sections[name] = std::chrono::high_resolution_clock::now();
}

void Profiler::endSection(const std::string &name)
{
    auto end = std::chrono::high_resolution_clock::now();
    auto startIt = sections.find(name);
    if (startIt != sections.end()) {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startIt->second).count();
        timings[name] += duration;
        sections.erase(startIt);
    }
}

void Profiler::logResults() const
{
    std::cout << "=== Profiling Results ===\n";
    for (const auto &[name, time] : timings) {
        double durationMs = static_cast<double>(time) / 1000.0;
        std::cout << "Section [" << name << "] took " << durationMs << "ms\n";
    }
    std::cout << "=========================\n";
}
