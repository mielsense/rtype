/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <map>
#include <memory>

#include "Scene.hpp"
#include "network/APacket.hpp"
#include "network/transport/ITransportPeer.hpp"
#include "scenes_network/ISceneNetwork.hpp"

enum class TransitionState { NONE, FADE_OUT, FADE_IN };

class SceneManager {
    private:
        std::map<SceneType, std::unique_ptr<Scene>> scenes; // scenes
        std::map<SceneType, std::unique_ptr<ISceneNetwork>> scene_networks;
        SceneType currentScene; // current scene
        SceneType targetScene;  // target scene

        sf::Clock sceneClock; // scene clock
        sf::RenderTexture fb; // frame buffer
        sf::View defaultView; // default view

        // transition
        sf::Clock transitionClock;
        float transitionDuration;
        TransitionState transitionState;
        sf::RectangleShape transitionRect;

    public:
        SceneManager();
        ~SceneManager() = default;

        // class methods
        void add(SceneType type, std::unique_ptr<Scene> scene, std::unique_ptr<ISceneNetwork> scene_networks = nullptr);
        void change(SceneType type);

        void initialize(unsigned int width, unsigned int height);
        void events(const sf::Event &event);

        void handler(bool hasFocus);
        void transition(float dt);

        bool handlePacket(const tetriq::APacket &packet, tetriq::ITransportPeer &client);

        // getters and setters
        const sf::Texture &getFrameBufferTexture() const;
        sf::RenderTexture &getFrameBuffer();
        SceneType getCurrentScene() const
        {
            return currentScene;
        }

}; // class SceneManager

#endif // SCENEMANAGER_HPP
