/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef CORE_HPP
#define CORE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <map>
#include "network/packets/NewPlayerPacket.hpp"
#include "network/transport/ITransportPeer.hpp"

#include "Logging.hpp"
#include "Registry.hpp"
#include "display/Scene.hpp"
#include "display/SceneManager.hpp"
#include "display/scenes/Game.hpp"
#include "display/scenes/Menu.hpp"
#include "editor/Editor.hpp"
#include "network/AClient.hpp"

class Core: public tetriq::AClient {
    private:
        // common
        bool running = true;    // to keep track of the running state
        ecs::Registry registry; // registry (=ecs)

        // game info
        int frameCount = 0;                           // frame count
        float frameUpdateTime = 0.0f;                 // frame update time
        sf::Vector2<float> windowSize = {0.0f, 0.0f}; // window size

        // editor
        Editor editor;

        // display
        SceneManager sm; // scene manager
        sf::RenderWindow window;
        sf::Sprite fbSprite;       // frame buffer sprite
        sf::Shader shader;         // shader
        std::string currentShader; // name of the current shader

        // server communication
        double lastTickTime = 0; // to keep track of the last tick time
        double currentTime = 0;  // current time in seconds

    public:
        Core(std::unique_ptr<tetriq::ITransportLayer> &transport, sf::Vector2<float> _windowSize, const std::string &_windowTitle,
             unsigned int _fpsLimit);
        ~Core();

        // class methods
        void run();
        void events();
        void update(sf::Time deltaTime);

        void calculateFps(float deltaTime);
        void initScenes();
        void initECS();

        bool handlePacket(const tetriq::APacket &packet, tetriq::ITransportPeer &client) override;
        bool loadShader(const std::string &shaderName);

        void reconnectToMaster();
}; // class Core

// helper functions
inline double ts(const std::chrono::duration<double> &duration)
{
    return duration.count();
}

#endif // CORE_HPP
