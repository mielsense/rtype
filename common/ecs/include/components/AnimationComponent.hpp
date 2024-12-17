/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AnimationComponent
*/

#ifndef ANIMATIONCOMPONENT_HPP_
#define ANIMATIONCOMPONENT_HPP_

#include <SFML/Graphics/Rect.hpp>
#include <cstdint>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Logging.hpp"
#ifdef ERROR
#undef ERROR_LOG  // Not recommended unless you're sure you won't use Windows GDI
#endif
namespace ecs {
    struct AnimationFrame {
            sf::IntRect frame;
            std::uint32_t frameIndex; // in the whole spritesheet
            float elapsedTime;
    };

    struct AnimationState {
            std::string key;
            std::vector<AnimationFrame> frames;
            bool repeat;
    };

    struct AnimationComponent {
            std::string currentState;
            std::size_t currentFrame; // in the current animation
            float timer;
            bool ended;
            std::unordered_map<std::string, AnimationState> states;
            std::string animationPath;
            std::string animationKey;

            void transitionTo(const std::string &state)
            {
                if (currentState == state)
                    return;
                currentState = state;
                currentFrame = 0;
                ended = false;
            }

            AnimationComponent(const std::string &path = "", const std::string &state = "IDLE", const std::string &key = "") :
                currentState(state), currentFrame(0), timer(0.0f), ended(false), animationPath(path), animationKey(key)
            {
                if (path.empty())
                    return;

                try {
                    // Open, read and parse file content
                    std::ifstream file(path);
                    if (!file.is_open()) {
                        LOG_ERROR("Failed to open animation file: {}", path);
                        return;
                    }

                    std::stringstream buffer;
                    buffer << file.rdbuf();
                    nlohmann::json json = nlohmann::json::parse(buffer.str());

                    // Check mandatory JSON keys
                    if (!json.contains("frameWidth") || !json.contains("frameHeight") || !json.contains("states")) {
                        LOG_ERROR("Invalid JSON structure: Missing required keys");
                        return;
                    }

                    int frameWidth = json.at("frameWidth").get<int>();
                    int frameHeight = json.at("frameHeight").get<int>();

                    const auto &stateArray = json.at("states");

                    for (const auto &state : stateArray) {
                        std::string key = state.at("key").get<std::string>();
                        bool repeat = state.at("repeat").get<bool>();
                        const auto &framesJson = state.at("frames");

                        std::vector<AnimationFrame> frames;
                        frames.reserve(framesJson.size()); // reserve to avoid reallocations

                        for (const auto &frame : framesJson) {
                            frames.emplace_back(AnimationFrame{
                                sf::IntRect(frame.at("frameIndex").get<std::uint32_t>() * frameWidth, 0, frameWidth, frameHeight),
                                frame.at("frameIndex").get<std::uint32_t>(), frame.at("elapsedTime").get<float>()});
                        }

                        states[key].key = key;
                        states[key].repeat = repeat;
                        states[key].frames = std::move(frames);
                    }
                } catch (const nlohmann::json::exception &e) {
                    LOG_ERROR("Failed to parse JSON: {}", e.what());
                }
            }
    };
} // namespace ecs

#endif /* !ANIMATIONCOMPONENT_HPP_ */
