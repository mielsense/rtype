/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** map handler for loading/saving game maps
*/

#ifndef MAPHANDLER_HPP
#define MAPHANDLER_HPP

#pragma once

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include "Registry.hpp"

#define MAP mh.map

struct Map {
        struct TextureData {
                sf::Texture texture;
                std::string path;
        };

        std::unordered_map<std::string, TextureData> textures;
        std::unordered_map<std::string, std::string> animations;
        std::string music;
        float width;
        float height;
};

enum class MapComponent {
    BasicInfo = 1 << 0,
    Assets = 1 << 1,
    Background = 1 << 2,
    Player = 1 << 3,
    Enemies = 1 << 4,
    All = BasicInfo | Assets | Background | Player | Enemies
};

inline MapComponent operator|(MapComponent a, MapComponent b)
{
    return static_cast<MapComponent>(static_cast<int>(a) | static_cast<int>(b));
}

inline MapComponent operator&(MapComponent a, MapComponent b)
{
    return static_cast<MapComponent>(static_cast<int>(a) & static_cast<int>(b));
}

inline bool operator!(MapComponent a)
{
    return static_cast<int>(a) == 0;
}

class MapHandler {
    public:
        struct PlayerData {
                sf::Vector2f position;
                sf::Vector2f scale;
                sf::Vector2f maxSpeed;
                sf::Vector2f hitboxOffset;
                std::string texture;
                std::string animation;
                std::string tag;
                float health;
        };

        struct EnemyData {
                sf::Vector2f position;
                sf::Vector2f velocity;
                sf::Vector2f scale;
                sf::Vector2f hitboxOffset;
                std::string texture;
                std::string animation;
                std::string tag;
                int health;
                bool showHitbox;
        };

    private:
        ecs::Registry &registry;
        nlohmann::ordered_json mapData;

        // helper methods for entity management
        void loadEntity(const nlohmann::ordered_json &entityData, ecs::Entity entity) const;
        nlohmann::ordered_json createOrderedEntityJson(ecs::Entity entity) const;

        std::string findTextureKey(const sf::Texture *texture) const;
        std::string findAnimationKey(const ecs::AnimationComponent &anim) const;

        // background helpers
        struct ElementKey {
                std::string type;
                std::string texture;
                float scrollSpeed;
                bool operator==(const ElementKey &other) const;
        };
        struct ElementKeyHash {
                std::size_t operator()(const ElementKey &k) const;
        };

        // save components with ordered formatting
        void saveAssets(nlohmann::ordered_json &mapData);
        void saveBackground(nlohmann::ordered_json &mapData);
        void saveMissile(nlohmann::ordered_json &mapData);
        void savePlayer(nlohmann::ordered_json &mapData);
        void saveEnemies(nlohmann::ordered_json &mapData);

    public:
        MapHandler(ecs::Registry &registry);
        ~MapHandler() = default;

        Map map;

        // main operations
        void load(const std::string &mapPath, MapComponent components = MapComponent::All);
        void save(const std::string &mapPath);

        // load components
        void loadBasicInfo();
        void loadAssets();
        void loadBackground();
        void loadPlayer();
        void loadEnemies();

        // spawn methods
        ecs::Entity spawnPlayer(const PlayerData &data);
        ecs::Entity spawnEnemy(const EnemyData &data);

        void addSpriteToPlayer(ecs::Entity player) const;
        void addSpriteToBullet(ecs::Entity bullet) const;
};

#endif // MAPHANDLER_HPP
