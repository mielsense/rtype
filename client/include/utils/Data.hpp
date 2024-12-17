/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef DATA_HPP
#define DATA_HPP

#pragma once

#define DATA gameInfo

#include "Entity.hpp"

// global game data
// mainly used by editor

struct GameInfo {
        float fps = 0.0f;                              // current fps
        int fpsLimit = 0;                              // fps limit
        bool scrollingBackground = true;               // scrolling background
        bool debug = false;                            // debug mode
        sf::Vector2<float> windowSize = {0, 0};        // window size
        sf::Vector2<float> targetWindowSize = {0, 0};  // target window size
        ecs::Entity selectedEntity = ecs::NULL_ENTITY; // selected entity
        bool startGame = false;                        // start game
        sf::Vector2<float> cameraCenter = {0, 0};      // camera center
        bool toggleMusic = false;                      // toggle music
        float musicVolume = 10.0f;                     // music volume
        std::string shader;                            // shader filename
        std::string playerName;                        // current player names
};

extern GameInfo gameInfo;

#endif // DATA_HPP
