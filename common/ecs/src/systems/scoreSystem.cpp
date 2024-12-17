/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ScoreSystem
*/

#include <iostream>
#include "ScoreComponent.hpp"

namespace ecs {

    //* Add points to the score and update high score if needed
    void scoreAddPointsSystem(ScoreComponent &score, int points) {
        score.score += points;
        if (score.score > score.highScore) {
            score.highScore = score.score;
        }
    }

    //* Reset score but keep high score
    void scoreResetSystem(ScoreComponent &score) {
        score.score = 0;
    }

}
