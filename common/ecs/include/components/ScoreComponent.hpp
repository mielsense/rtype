/*
** EPITECH PROJECT, 2024
** ScoreComponent
** File description:
** ScoreComponent
*/

#pragma once

namespace ecs {
    struct ScoreComponent {
            int score;
            int highScore;

            //* Constructor with initial score and optional highscore
            ScoreComponent(int initialScore = 0, int initialHighScore = 0)
                : score(initialScore), highScore(initialHighScore) {}
    };
}