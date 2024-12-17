/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef EMPTY_HPP
#define EMPTY_HPP

#pragma once

#include "display/Scene.hpp"

class Empty: public Scene {
    private:
    public:
        Empty(sf::RenderTexture &fb, ecs::Registry &registry, SceneChangeCallback changeSceneCallback) :
            Scene(fb, registry, changeSceneCallback)
        {}
        ~Empty() = default;

        // class methods

        void init() override;
        void events(const sf::Event &event) override;
        void update(float dt, bool hasFocus) override;
        void draw(float dt) override;
        void onChange() override;
};

#endif // EMPTY_HPP
