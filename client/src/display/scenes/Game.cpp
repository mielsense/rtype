/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game
*/

#include "display/scenes/Game.hpp"
#include <imgui.h>
#include <random>
#include <utility>
#include "Logging.hpp"
#include "network/transport/ITransportLayer.hpp"
#include "systems.hpp"
#include "utils/Data.hpp"

Game::Game(sf::RenderTexture &fb, ecs::Registry &registry, std::function<void()> reconnectCallback,
           SceneChangeCallback changeSceneCallback) : Scene(fb, registry, std::move(changeSceneCallback)), mh(registry), _reconnectCallback(std::move(reconnectCallback))
{
}

void Game::init()
{
    mh.load("client/assets/maps/map-1.json",
            MapComponent::BasicInfo | MapComponent::Assets | MapComponent::Background | MapComponent::Player);

    // load base player name
    ecs::Entity player = registry.getEntityByTag("player");
    try {
        auto playerComp = registry.tryGet<ecs::PlayerComponent>(player);
        DATA.playerName = playerComp.name;
    } catch (...) {
        DATA.playerName = "Player ?";
    }

    // Initialize camera
    const ecs::Entity camera = registry.createEntity();
    registry.set<ecs::CameraComponent>(camera, ecs::CameraComponent(DATA.windowSize.x, DATA.windowSize.y, MAP.width));
    registry.tagEntity(camera, "mainCamera");

    // Healthbar component
    ecs::Entity healthBar = registry.createEntity();
    registry.set<ecs::UIComponent>(healthBar, ecs::UIComponent(DATA.windowSize));
    registry.set<ecs::PositionComponent>(healthBar, ecs::PositionComponent(DATA.windowSize.x - 200.0f, DATA.windowSize.y - 70.0f));
    registry.set<ecs::SpriteComponent>(healthBar, ecs::SpriteComponent(MAP.textures["healthbar"].texture, {4.0f, 4.0f}));
    registry.set<ecs::AnimationComponent>(healthBar, ecs::AnimationComponent(MAP.animations["healthbar"], "FULL"));

    const ecs::Entity playerId = registry.getEntityByTag("player");
    registry.set<ecs::UIFollowComponent>(healthBar, ecs::UIFollowComponent(playerId));

    //* Score component
    ecs::Entity scoreEntity = registry.createEntity();
    auto &ui = registry.set<ecs::UIComponent>(scoreEntity, ecs::UIComponent(DATA.windowSize));
    ui.setTextStyle(sf::Color::White, 30);
    registry.set<ecs::PositionComponent>(scoreEntity, ecs::PositionComponent(DATA.windowSize.x - 200.0f, 20.0f)); //? Top right corner
    registry.set<ecs::SpriteComponent>(scoreEntity, ecs::SpriteComponent());
    registry.set<ecs::ScoreComponent>(scoreEntity);
    registry.tagEntity(scoreEntity, "score");


    // Initialize audio
    if (!music.openFromFile(MAP.music)) {
        LOG_WARNING("Failed to load music");
    } else {
        music.setLoop(true);
        music.setVolume(DATA.musicVolume);
    }

    // Initialize UI font
    if (!uiFont.loadFromFile("client/assets/font-light.ttf")) {
        LOG_WARNING("Failed to load UI font");
    }

    initialView = getCamera().view;
}

void Game::events(const sf::Event &event)
{
    if (event.type == sf::Event::MouseMoved) {
        registry.forEach<ecs::PositionComponent, ecs::SpriteComponent, ecs::UIComponent>(
            [&](ecs::PositionComponent &pos, ecs::SpriteComponent &sprite, ecs::UIComponent &ui)
            {
                const sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
                UISystem(ui, pos, sprite, mousePos);
            });
    }
}

void Game::update(float dt, bool hasFocus)
{
    if (!DATA.debug) {
        updateGameplay(dt, hasFocus);
        updateCamera(dt);
    }
    updateMusic();
    updateHitboxes();
    registry.deleteFlaggedEntities();
}

void Game::draw(float dt)
{
    if (!DATA.debug) {
        drawBackground(dt);
        drawEntities();
        drawUI();
    }
}

void Game::onChange()
{}

ecs::CameraComponent &Game::getCamera() const
{
    return registry.tryGet<ecs::CameraComponent>("mainCamera");
}

void Game::updateCamera(float dt)
{
    auto &camera = getCamera();
    if (DATA.startGame && !camera.isScrolling) {
        startGame();
    }
    DATA.cameraCenter = camera.view.getCenter();
    ecs::cameraSystem(camera, dt);
    fb.setView(camera.view);
}

void Game::startGame()
{
    if (gameStarted)
        return;

    auto &camera = getCamera();
    gameStarted = true;
    camera.isScrolling = true;
}

void Game::updateGameplay(float dt, bool hasFocus)
{
    if (hasFocus) {
        registry.forEach<ecs::ControlMoveComponent, ecs::VelocityComponent>(
            [&](ecs::Entity id, ecs::ControlMoveComponent &ctrl, ecs::VelocityComponent &vel)
            {
                try {
                    auto player = &registry.tryGet<ecs::PlayerComponent>(id);
                    if (player->name != DATA.playerName) {
                        player->setName(DATA.playerName);
                    }
                } catch (...) {
                }

                controlMoveSystem(ctrl, vel);
                if (vel.vx != 0 || vel.vy != 0)
                    client_actions.emplace(Action::MOVE);
            });
        registry.forEach<ecs::ControlMoveComponent, ecs::AnimationComponent>(
            [](ecs::ControlMoveComponent &ctrl, ecs::AnimationComponent &anim) { controlAnimationStateSystem(ctrl, anim); });
        registry.forEach<ecs::ControlShootComponent, ecs::PositionComponent, ecs::HitboxComponent>(
            [&, dt](ecs::ControlShootComponent &shoot, ecs::PositionComponent, ecs::HitboxComponent &)
            {
                if (controlShootSystem(shoot, dt)) {
                    client_actions.emplace(Action::SHOOT);
                }
            });
    }

    registry.forEach<ecs::PositionComponent, ecs::VelocityComponent>([dt](ecs::PositionComponent &pos, ecs::VelocityComponent &vel)
                                                                     { positionSystem(pos, vel, dt); });

    const sf::View view = getCamera().view;
    std::vector<ecs::Entity> shoots;

    registry.forEach<ecs::PositionComponent, ecs::HitboxComponent, ecs::SpriteComponent, ecs::ShootComponent>(
        [&](ecs::Entity id, ecs::PositionComponent &pos, ecs::HitboxComponent &hitbox, ecs::SpriteComponent &sprite, ecs::ShootComponent &)
        {
            if (sprite.sprite.getTextureRect().width == 0 || sprite.sprite.getTextureRect().height == 0)
                return;
            if (pos.x < view.getCenter().x + view.getSize().x / 2 &&
                pos.x + hitbox.rect.width > view.getCenter().x - view.getSize().x / 2 &&
                pos.y < view.getCenter().y + view.getSize().y / 2 &&
                pos.y + hitbox.rect.height > view.getCenter().y - view.getSize().y / 2) {
                shoots.push_back(id);
            }
        });

    registry.forEach<ecs::AnimationComponent, ecs::SpriteComponent>([dt](ecs::AnimationComponent &anim, ecs::SpriteComponent &sprite)
                                                                    { animationSystem(anim, sprite, dt); });
    auto &camera = getCamera();
    registry.forEach<ecs::PositionComponent, ecs::VelocityComponent, ecs::HitboxComponent, ecs::PlayerComponent>(
        [dt, &camera](ecs::PositionComponent &pos, ecs::VelocityComponent &vel, ecs::HitboxComponent &hitbox, ecs::PlayerComponent &)
        { playerSystem(pos, vel, camera, dt, hitbox); });
}

void Game::updateHitboxes()
{
    registry.forEach<ecs::PositionComponent, ecs::SpriteComponent, ecs::HitboxComponent>(
        [](ecs::PositionComponent &pos, ecs::SpriteComponent &sprite, ecs::HitboxComponent &hitbox)
        { hitboxSystem(pos, sprite, hitbox, DATA.debug); });
}

void Game::updateMusic()
{
    if (DATA.toggleMusic) {
        if (music.getVolume() != DATA.musicVolume) {
            music.setVolume(DATA.musicVolume);
        }
        if (music.getStatus() != sf::Music::Playing) {
            music.play();
        }
    } else {
        if (music.getStatus() == sf::Music::Playing) {
            music.pause();
        }
    }
}

void Game::drawBackground(float dt)
{
    const auto &camera = getCamera();
    registry.forEach<ecs::BackgroundComponent>(
        [&](ecs::BackgroundComponent &bg)
        {
            bg.isScrolling = camera.isScrolling;
            if (camera.finalStop)
                bg.isScrolling = false;

            if (DATA.scrollingBackground)
                backgroundSystem(bg, dt, camera.view);
            drawBackgroundSystem(fb, bg, camera.view);
        });
}

void Game::drawEntities()
{
    auto &camera = getCamera();
    registry.forEach<ecs::PositionComponent, ecs::SpriteComponent, ecs::HitboxComponent>(
        [&](ecs::Entity entity, ecs::PositionComponent &pos, ecs::SpriteComponent &sprite, ecs::HitboxComponent &hitbox)
        {
            ecs::PlayerComponent *player = nullptr;
            try {
                player = &registry.tryGet<ecs::PlayerComponent>(entity);
            } catch (...) {
                player = nullptr;
            }
            renderSystem(fb, pos, sprite, hitbox, player, camera.view, uiFont);
        });
}

void Game::drawUI()
{
    registry.forEach<ecs::UIComponent, ecs::UIFollowComponent>(
        [&](ecs::Entity entity, ecs::UIComponent &ui, ecs::UIFollowComponent &follow)
        {
            auto &anim = registry.tryGet<ecs::AnimationComponent>(entity);
            auto &targetHealth = registry.tryGet<ecs::HealthComponent>(follow.targetEntity);

            float healthPercent = targetHealth.health / targetHealth.maxHealth;

            // Update tooltip using fmt
            ui.tooltip = fmt::format("Health: {}/{}", static_cast<int>(targetHealth.health), static_cast<int>(targetHealth.maxHealth));

            std::string stateKey;
            if (healthPercent <= 0.0f)
                stateKey = "EMPTY";
            else if (healthPercent <= 0.25f)
                stateKey = "LOW";
            else if (healthPercent <= 0.50f)
                stateKey = "HALF";
            else if (healthPercent <= 0.75f)
                stateKey = "HIGH";
            else
                stateKey = "FULL";

            anim.transitionTo(stateKey);
        });
    //* Score update
    registry.forEach<ecs::UIComponent, ecs::ScoreComponent>(
        [](ecs::UIComponent &ui, ecs::ScoreComponent &score)
        {
            // TODO ADD HIGH SCORE
            //            ui.mainText = fmt::format("Score: {}\nHigh Score: {}", score.score, score.highScore);
            ui.mainText = fmt::format("Score: {}", score.score);
        });
    registry.forEach<ecs::PositionComponent, ecs::SpriteComponent, ecs::UIComponent>(
        [&](ecs::PositionComponent &pos, ecs::SpriteComponent &sprite, ecs::UIComponent &ui)
        { UIrenderSystem(fb, pos, sprite, ui, uiFont); });
}

const MapHandler &Game::getMapHandler() const
{
    return mh;
}

std::queue<Game::Action> &Game::getClientActions()
{
    return client_actions;
}

void Game::reset()
{
//    stopGame();
}

void Game::stopGame()
{
    try {
        gameStarted = false;
        client_actions = std::queue<Action>();

        if (music.getStatus() == sf::Music::Playing) {
            music.stop();
        }

        LOG_INFO("Game state reset complete");
    } catch (const std::exception& e) {
        LOG_ERROR("Error stopping game: {}", e.what());
    }
}