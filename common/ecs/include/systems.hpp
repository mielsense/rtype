/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** systems
*/

#ifndef SYSTEMS_HPP_
#define SYSTEMS_HPP_

#include "components.hpp"

namespace ecs {
    void animationSystem(AnimationComponent &anim, SpriteComponent &sprite, float dt);
    void controlAnimationStateSystem(const ControlMoveComponent &ctrl, AnimationComponent &anim);
    void backgroundSystem(BackgroundComponent &bg, float dt, const sf::View &view);
    void drawBackgroundSystem(sf::RenderTarget &target, BackgroundComponent &bg, const sf::View &view);
    void controlMoveSystem(const ControlMoveComponent &ctrl, VelocityComponent &vel);
    void positionSystem(PositionComponent &pos, const VelocityComponent &vel, float dt);
    void renderSystem(sf::RenderTexture &fb, const PositionComponent &pos, SpriteComponent &sprite, const HitboxComponent &hitbox,
                      const PlayerComponent *player, const sf::View &cameraView, const sf::Font &font);
    bool controlShootSystem(ControlShootComponent &shoot, float dt);
    void hitboxSystem(const PositionComponent &pos, const SpriteComponent &sprite, HitboxComponent &hitbox, bool isEditor = false);
    void cameraSystem(CameraComponent &camera, float dt);
    void playerSystem(PositionComponent &pos, VelocityComponent &vel, const CameraComponent &camera, float dt,
                      const HitboxComponent &hitbox);
    Entity collisionSystem(Entity id, PositionComponent &pos, HitboxComponent &hitbox, EnemyComponent &enemy,
                           const std::vector<Entity> &shoots, Registry &registry);
    void cleanupSystem(Registry &registry, const sf::View &cameraView);
    void UIrenderSystem(sf::RenderTexture &fb, PositionComponent &pos, SpriteComponent &sprite, UIComponent &ui, const sf::Font &font);
    void UISystem(UIComponent &ui, PositionComponent &pos, SpriteComponent &sprite, const sf::Vector2f &mousePos);
    void scoreAddPointsSystem(ScoreComponent &score, int points);
    void scoreResetSystem(ScoreComponent &score);
    bool shootDamageSystem(Entity targetEntity, Entity shootEntity, Registry &registry);
    bool shootLifetimeSystem(ShootComponent &shoot, float dt);

} // namespace ecs

#endif /* !SYSTEMS_HPP_ */
