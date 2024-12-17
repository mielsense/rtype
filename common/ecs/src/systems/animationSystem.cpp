/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** animationSystem
*/

#include "AnimationComponent.hpp"
#include "SpriteComponent.hpp"

namespace ecs {
    void animationSystem(AnimationComponent &anim, SpriteComponent &sprite, const float dt)
    {
        std::size_t &frameIndex = anim.currentFrame;
        AnimationState &animState = anim.states[anim.currentState];

        anim.transitionTo(animState.key);

        if (frameIndex >= animState.frames.size()) {
            if (!animState.repeat) {
                if (anim.ended)
                    return;
                sprite.sprite.setTextureRect(animState.frames.back().frame);
                anim.ended = true;
                return;
            }
            frameIndex = 0;
        }

        AnimationFrame &currentAnimFrame = animState.frames[frameIndex];

        sprite.sprite.setTextureRect(currentAnimFrame.frame);

        // Handle animation speed
        anim.timer += dt;

        if (anim.timer >= currentAnimFrame.elapsedTime) {
            frameIndex++;
            anim.timer = 0.0f;
        }
    }
} // namespace ecs
