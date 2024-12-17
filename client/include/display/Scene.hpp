/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Event.hpp>

#include "Registry.hpp"

enum class SceneType { MENU, GAME, EMPTY };

using SceneChangeCallback = std::function<void(SceneType)>;

class Scene
{
public:
    sf::RenderTexture& fb;

protected:
    ecs::Registry& registry;

public:
    Scene(sf::RenderTexture& buffer, ecs::Registry& registry, SceneChangeCallback changeSceneCallback) :
        fb(buffer), registry(registry), changeScene(changeSceneCallback)
    {
    }

    virtual ~Scene() = default;

    SceneChangeCallback changeScene;

    // class methods
    virtual void init() = 0;
    virtual void update(float dt, bool hasFocus) = 0;
    virtual void draw(float dt) = 0;
    virtual void events(const sf::Event& event) = 0;
    virtual void onChange() = 0;
};

#endif // SCENE_HPP
