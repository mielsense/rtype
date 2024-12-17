/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <queue>
#include <utility>

#include "display/Scene.hpp"
#include "map/MapHandler.hpp"

class Game final: public Scene {
    public:
        enum class Action { SHOOT, MOVE };

    private:
        bool gameStarted = false;
        MapHandler mh;
        sf::Music music;
        std::queue<Action> client_actions;



        sf::Font uiFont;

        void updateCamera(float dt);
        void updateGameplay(float dt, bool hasFocus);
        void updateHitboxes();
        void updateMusic();
        void drawBackground(float dt);
        void drawEntities();
        void drawUI();

    public:

        Game(sf::RenderTexture &fb, ecs::Registry &registry,
            std::function<void()> reconnectCallback,
            SceneChangeCallback changeSceneCallback);

        // class methods

        void init() override;
        void events(const sf::Event &event) override;
        void update(float dt, bool hasFocus) override;
        void draw(float dt) override;


        void reset();

        void onChange() override;

        void startGame();

        // getters
        const MapHandler &getMapHandler() const;
        ecs::CameraComponent &getCamera() const;
        std::queue<Action> &getClientActions();
        void stopGame();

        std::function<void()> _reconnectCallback;
        sf::View initialView;
};

#endif // GAME_HPP
