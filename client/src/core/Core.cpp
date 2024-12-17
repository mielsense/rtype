/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifdef THROW_ERROR
#undef THROW_ERROR // Not recommended unless you're sure you won't use Windows GDI
#endif
#include "core/Core.hpp"
#include "Logging.hpp"
#include "display/scenes/Empty.hpp"
#include "scenes_network/GameNetwork.hpp"
#include "scenes_network/MenuNetwork.hpp"
#include "utils/ConnectionConfig.hpp"
#include "utils/Data.hpp"
#include "utils/Error.hpp"

#include <imgui-SFML.h>
#include <imgui.h>
#include <network/transport/TransportLayerFactory.hpp>

GameInfo gameInfo; // global game info (= DATA)

Core::Core(std::unique_ptr<tetriq::ITransportLayer> &transport, sf::Vector2<float> _windowSize, const std::string &_windowTitle,
           unsigned int _fpsLimit) :
    AClient(transport), windowSize(_windowSize), editor(windowSize,
                                                        [&](SceneType scene)
                                                        {
                                                            DATA.debug = false;
                                                            sm.change(scene);
                                                        }),
    window(sf::VideoMode(windowSize.x, windowSize.y), _windowTitle)
{
    // check if window was created
    if (!window.isOpen()) {
        THROW_ERROR("Failed to create window");
    }
    if (!ImGui::SFML::Init(window)) {
        THROW_ERROR("Failed to initialize ImGui");
    }

    // set window properties
    window.setKeyRepeatEnabled(false);
    if (_fpsLimit != 0) {
        window.setFramerateLimit(_fpsLimit);
        DATA.fpsLimit = static_cast<int>(_fpsLimit);
    }

    DATA.shader = "dithering.fs";
    if (!loadShader(DATA.shader)) {
        THROW_ERROR("Failed to load initial shader");
    }

    // initalize necessary
    DATA.windowSize = windowSize;
    DATA.targetWindowSize = windowSize;

    initECS();
    initScenes();
    editor.initialize();
}

Core::~Core()
{
    ImGui::SFML::Shutdown();
}

void Core::events()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window, event);

        // window events
        if (event.type == sf::Event::Closed)
            window.close();

        // user events
        if (event.type == sf::Event::KeyPressed) {
            // TODO : user events
        }

        // handler scene events
        sm.events(event);
    }
}

void Core::run()
{
    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Time deltaTime = deltaClock.restart();
        ImGui::SFML::Update(window, deltaTime);

        // handle window and user events
        events();
        update(deltaTime);

        // handle logic that runs every tick
        float dt = deltaTime.asSeconds();
        loop(dt);

        // scene manager

        sm.handler(window.hasFocus());
        registry.deleteFlaggedEntities();
        fbSprite.setTexture(sm.getFrameBufferTexture());

        // render
        window.clear(sf::Color::White);
        if (DATA.shader.empty()) {
            window.draw(fbSprite);
        } else {
            window.draw(fbSprite, &shader);
        }

        // Draw editor only for game scene
        if (DATA.debug) {
            editor.render(sm.getFrameBuffer());
        }

        // Render ImGui at the end of the frame
        ImGui::SFML::Render(window);
        window.display();
    }
}

void Core::update(sf::Time deltaTime)
{
    calculateFps(deltaTime.asSeconds());

    if (DATA.targetWindowSize != DATA.windowSize) {
        window.setSize({(unsigned int) DATA.targetWindowSize.x, (unsigned int) DATA.targetWindowSize.y});
    }
    windowSize = static_cast<sf::Vector2<float>>(window.getSize());
    DATA.windowSize = windowSize;
    window.setFramerateLimit(DATA.fpsLimit);

    // Check if shader needs to be updated
    if (currentShader != DATA.shader) {
        loadShader(DATA.shader);
    }
    if (currentShader == "dithering.fs") {
        shader.setUniform("resolution", sf::Glsl::Vec2(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
    }
}

void Core::calculateFps(float deltaTime)
{
    frameCount++;
    frameUpdateTime += deltaTime;

    if (frameUpdateTime >= 1.0f) {
        DATA.fps = frameCount;
        frameCount = 0;
        frameUpdateTime = 0.0f;
    }
}

void Core::initScenes()
{
    std::function connectToGame = [this](std::string serverAddress, uint16_t port)
    {
        std::string adress = serverAddress;
        if (serverAddress == "0.0.0.0") {
            adress = "localhost";
        }
        sm.change(SceneType::GAME);
        auto new_transport = tetriq::TransportLayerFactory::createClient(adress, port, "GameClient");
        _transport.swap(new_transport);
    };

    std::function reconnectToMaster = [this]()
    {
        static bool isReconnecting = false;
        if (isReconnecting) {
            LOG_WARNING("Already reconnecting, skipping...");
            return;
        }

        isReconnecting = true;
        LOG_INFO("Starting reconnection process");

        try {
            //* Create new transport first
            auto& config = ConnectionConfig::getInstance();
            auto new_transport = tetriq::TransportLayerFactory::createClient(
                config.masterIp,
                config.masterPort,
                config.clientName
            );

            //* Swap transport
            _transport.swap(new_transport);
            LOG_INFO("Transport swapped");

            LOG_INFO("Successfully reconnected to master server");
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to reconnect to master server: {}", e.what());
        }

        isReconnecting = false;
    };

    std::function<void(SceneType)> changeScene = [&](SceneType scene) { sm.change(scene); };

    // add all scenes
    std::unique_ptr<Menu> menu = std::make_unique<Menu>(sm.getFrameBuffer(), registry, connectToGame, changeScene);
    auto menuNetwork = std::make_unique<MenuNetwork>(registry, _transport, *menu);
    sm.add(SceneType::MENU, std::move(menu), std::move(menuNetwork));

    std::unique_ptr<Game> game = std::make_unique<Game>(sm.getFrameBuffer(), registry, reconnectToMaster, changeScene);
    auto gameNetwork = std::make_unique<GameNetwork>(_transport, registry, *game);
    sm.add(SceneType::GAME, std::move(game), std::move(gameNetwork));

    sm.add(SceneType::EMPTY, std::make_unique<Empty>(sm.getFrameBuffer(), registry, changeScene));

    // init and set default scene
    sm.initialize(windowSize.x, windowSize.y);
    sm.change(SceneType::MENU);

    LOG(LogLevel::INFO, "Scenes initialized");
}

void Core::initECS()
{
    registry.add<ecs::AnimationComponent>();
    registry.add<ecs::BackgroundComponent>();
    registry.add<ecs::CameraComponent>();
    registry.add<ecs::ControlMoveComponent>();
    registry.add<ecs::ControlShootComponent>();
    registry.add<ecs::EnemyComponent>();
    registry.add<ecs::HealthComponent>();
    registry.add<ecs::HitboxComponent>();
    registry.add<ecs::PlayerComponent>();
    registry.add<ecs::PositionComponent>();
    registry.add<ecs::ShootComponent>();
    registry.add<ecs::SpriteComponent>();
    registry.add<ecs::UIComponent>();
    registry.add<ecs::UIFollowComponent>();
    registry.add<ecs::VelocityComponent>();
    registry.add<ecs::ScoreComponent>();
}

bool Core::handlePacket(const tetriq::APacket &packet, tetriq::ITransportPeer &client)
{
    return sm.handlePacket(packet, client);
}

bool Core::loadShader(const std::string &shaderName)
{
    if (shaderName.empty()) {
        currentShader = "";
        return true;
    }

    std::string shaderPath = "client/assets/shaders/" + shaderName;
    if (shader.loadFromFile(shaderPath, sf::Shader::Fragment)) {
        if (shaderName == "dithering.fs") {
            shader.setUniform("resolution", sf::Glsl::Vec2(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
        }
        currentShader = shaderName;
        LOG(LogLevel::INFO, "Shader loaded: {}", shaderPath);
        return true;
    }
    LOG(LogLevel::ERROR_LOG, "Failed to load shader: {}", shaderPath);
    return false;
}

void Core::reconnectToMaster()
{
    auto &config = ConnectionConfig::getInstance();
    try {
        _transport = tetriq::TransportLayerFactory::createClient(config.masterIp, config.masterPort, config.clientName);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to reconnect to master server: {}", e.what());
        throw;
    }
}
