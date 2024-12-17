/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#include "display/SceneManager.hpp"
#include "Logging.hpp"
#include "utils/Data.hpp"
#ifdef THROW_ERROR
#undef THROW_ERROR // Not recommended unless you're sure you won't use Windows GDI
#endif
SceneManager::SceneManager() :
    currentScene(SceneType::EMPTY), targetScene(SceneType::EMPTY), transitionDuration(0.5f),
    transitionState(TransitionState::NONE)
{
    transitionRect.setFillColor(sf::Color::Black);
}

void SceneManager::add(SceneType type, std::unique_ptr<Scene> scene, std::unique_ptr<ISceneNetwork> scene_network)
{
    scenes[type] = std::move(scene);
    if (scene_network != nullptr) {
        scene_networks[type] = std::move(scene_network);
    }
    LOG(LogLevel::DEBUG, "Scene added: {}", static_cast<int>(type));
}

void SceneManager::change(SceneType type)
{
    if (type == currentScene) {
        return;
    }
    if (scenes.find(type) != scenes.end() && type != currentScene && transitionState == TransitionState::NONE) {
        targetScene = type;
        currentScene = type;

        transitionState = TransitionState::FADE_OUT;
        transitionClock.restart();

        scenes[currentScene]->onChange();
        LOG(LogLevel::INFO, "Scene transition started: {} -> {}", static_cast<int>(currentScene),
            static_cast<int>(targetScene));
    } else {
        LOG(LogLevel::WARNING, "Scene change not initiated: type={}, current={}, state={}", static_cast<int>(type),
            static_cast<int>(currentScene), static_cast<int>(transitionState));
    }
}

void SceneManager::initialize(unsigned int width, unsigned int height)
{
    if (!fb.create(width, height)) {
        LOG(LogLevel::ERROR_LOG, "Failed to create frame buffer");
    }

    transitionRect.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));

    for (auto &[_, scene] : scenes) {
        scene->init();
    }
    LOG(LogLevel::INFO, "SceneManager initialized with dimensions: {}x{}", width, height);
}

void SceneManager::events(const sf::Event &event)
{
    if (scenes.find(currentScene) != scenes.end()) {
        scenes[currentScene]->events(event);
    } else {
        LOG(LogLevel::WARNING, "Current scene not found for events: {}", static_cast<int>(currentScene));
    }
}

void SceneManager::handler(bool hasFocus)
{
    fb.clear(sf::Color::Black);

    float deltaTime = sceneClock.restart().asSeconds();
    if (scenes.find(currentScene) != scenes.end()) {
        if (transitionState == TransitionState::NONE || transitionState == TransitionState::FADE_IN) {
            scenes[currentScene]->update(deltaTime, hasFocus);
            if (scene_networks.find(currentScene) != scene_networks.end()) {
                scene_networks[currentScene]->update(deltaTime);
            }
            scenes[currentScene]->draw(deltaTime);
        }
    } else {
        LOG(LogLevel::WARNING, "Current scene not found for update/draw: {}", static_cast<int>(currentScene));
    }

    transition(deltaTime);
    fb.display();
}

void SceneManager::transition(float)
{
    if (transitionState == TransitionState::NONE) {
        return;
    }

    float transitionTime = transitionClock.getElapsedTime().asSeconds();
    switch (transitionState) {
        case TransitionState::FADE_OUT:
            if (transitionTime < transitionDuration) {
                float alpha = (transitionTime / transitionDuration) * 255;
                transitionRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
            } else {
                transitionState = TransitionState::FADE_IN;
                transitionClock.restart();
                LOG(LogLevel::INFO, "Fade out complete, starting fade in");
            }
            break;
        case TransitionState::FADE_IN:
            if (transitionTime < transitionDuration) {
                float alpha = 255 - (transitionTime / transitionDuration) * 255;
                transitionRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
            } else {
                transitionState = TransitionState::NONE;
                LOG(LogLevel::INFO, "Fade in complete, transition finished");
            }
            break;
        default:
            break;
    }

    fb.draw(transitionRect);
}

bool SceneManager::handlePacket(const tetriq::APacket &packet, tetriq::ITransportPeer &client)
{
    return scene_networks[currentScene]->handlePacket(packet, client);
}

const sf::Texture &SceneManager::getFrameBufferTexture() const
{
    return fb.getTexture();
}

sf::RenderTexture &SceneManager::getFrameBuffer()
{
    return fb;
}
