/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** map handler implementation with modular entity management
*/

#include "map/MapHandler.hpp"
#include <fstream>
#include "Logging.hpp"
#include "components.hpp"

bool MapHandler::ElementKey::operator==(const ElementKey &other) const
{
    return type == other.type && texture == other.texture && scrollSpeed == other.scrollSpeed;
}

std::size_t MapHandler::ElementKeyHash::operator()(const ElementKey &k) const
{
    return std::hash<std::string>()(k.type) ^ (std::hash<std::string>()(k.texture) << 1) ^ (std::hash<float>()(k.scrollSpeed) << 2);
}

MapHandler::MapHandler(ecs::Registry &registry) : registry(registry)
{}

// helper methods for finding asset keys
std::string MapHandler::findTextureKey(const sf::Texture *texture) const
{
    for (const auto &[key, tex] : map.textures) {
        if (texture == &tex.texture)
            return key;
    }
    return "";
}

std::string MapHandler::findAnimationKey(const ecs::AnimationComponent &anim) const
{
    return anim.animationKey;
}

// spawn methods
ecs::Entity MapHandler::spawnPlayer(const PlayerData &data)
{
    ecs::Entity player = registry.createEntity();

    registry.set<ecs::PositionComponent>(player, ecs::PositionComponent(data.position.x, data.position.y));
    registry.set<ecs::VelocityComponent>(player, ecs::VelocityComponent(0.0f, 0.0f, data.maxSpeed.x, data.maxSpeed.y));

    if (!data.texture.empty() && map.textures.find(data.texture) != map.textures.end()) {
        registry.set<ecs::SpriteComponent>(player, ecs::SpriteComponent(map.textures.at(data.texture).texture, data.scale));
    }

    if (!data.animation.empty() && map.animations.find(data.animation) != map.animations.end()) {
        registry.set<ecs::AnimationComponent>(player, ecs::AnimationComponent(map.animations.at(data.animation), "IDLE", data.animation));
    }

    if (registry.has<ecs::SpriteComponent>(player)) {
        registry.set<ecs::HitboxComponent>(player,
                                           ecs::HitboxComponent(registry.tryGet<ecs::SpriteComponent>(player).sprite, data.hitboxOffset));
    }

    registry.set<ecs::PlayerComponent>(player, ecs::PlayerComponent(fmt::format("Player {}", player)));
    registry.set<ecs::ControlMoveComponent>(player, ecs::ControlMoveComponent());
    registry.set<ecs::ControlShootComponent>(player, ecs::ControlShootComponent());
    registry.set<ecs::HealthComponent>(player, ecs::HealthComponent(data.health));

    if (!data.tag.empty()) {
        registry.tagEntity(player, data.tag);
    }

    registry.tagEntity(player, "player");
    return player;
}

ecs::Entity MapHandler::spawnEnemy(const EnemyData &data)
{
    ecs::Entity enemy = registry.createEntity();

    registry.set<ecs::PositionComponent>(enemy, ecs::PositionComponent(data.position.x, data.position.y));
    registry.set<ecs::VelocityComponent>(enemy, ecs::VelocityComponent(data.velocity.x, data.velocity.y));

    if (!data.texture.empty() && map.textures.find(data.texture) != map.textures.end()) {
        registry.set<ecs::SpriteComponent>(enemy, ecs::SpriteComponent(map.textures.at(data.texture).texture, data.scale));
        auto &hitbox = registry.set<ecs::HitboxComponent>(
            enemy, ecs::HitboxComponent(registry.tryGet<ecs::SpriteComponent>(enemy).sprite, data.hitboxOffset));
        hitbox.renderable = data.showHitbox;
    } else {
        if (data.texture == "boss")
            registry.set<ecs::HitboxComponent>(enemy, ecs::HitboxComponent{1768, 2000, {2, 2}});
        else
            registry.set<ecs::HitboxComponent>(enemy, ecs::HitboxComponent{64, 64, {2, 2}});
    }

    if (!data.animation.empty() && map.animations.find(data.animation) != map.animations.end()) {
        registry.set<ecs::AnimationComponent>(enemy, ecs::AnimationComponent(map.animations.at(data.animation), "IDLE", data.animation));
    }

    if (!data.tag.empty()) {
        registry.tagEntity(enemy, data.tag);
    }

    if (!data.texture.empty()) {
        registry.set<ecs::EnemyComponent>(enemy, ecs::EnemyComponent(data.texture));
    } else {
        registry.set<ecs::EnemyComponent>(enemy, ecs::EnemyComponent("unknown"));
    }
    registry.set<ecs::HealthComponent>(enemy, ecs::HealthComponent(data.health));

    return enemy;
}

// generic entity management
void MapHandler::loadEntity(const nlohmann::ordered_json &entityData, ecs::Entity entity) const
{
    //    float posX = entityData.value("position", std::vector<float>{0.0f, 0.0f})[0];
    //    float posY = entityData.value("position", std::vector<float>{0.0f, 0.0f})[1];
    //    registry.set<ecs::PositionComponent>(entity, ecs::PositionComponent(posX, posY));

    std::string texture = entityData.value("texture", "");
    float scaleX = entityData.value("scale", std::vector<float>{1.0f, 1.0f})[0];
    float scaleY = entityData.value("scale", std::vector<float>{1.0f, 1.0f})[1];

    if (!texture.empty() && map.textures.find(texture) != map.textures.end()) {
        registry.set<ecs::SpriteComponent>(entity, ecs::SpriteComponent(map.textures.at(texture).texture, {scaleX, scaleY}));
    }

    std::string animation = entityData.value("animation", "");
    if (!animation.empty() && map.animations.find(animation) != map.animations.end()) {
        registry.set<ecs::AnimationComponent>(entity, ecs::AnimationComponent(map.animations.at(animation), "IDLE", animation));
    }

    float hitboxOffsetX = entityData.value("hitboxOffset", std::vector<float>{0.0f, 0.0f})[0];
    float hitboxOffsetY = entityData.value("hitboxOffset", std::vector<float>{0.0f, 0.0f})[1];
    if (registry.has<ecs::SpriteComponent>(entity)) {
        registry.set<ecs::HitboxComponent>(
            entity, ecs::HitboxComponent(registry.tryGet<ecs::SpriteComponent>(entity).sprite, {hitboxOffsetX, hitboxOffsetY}));
    }
}

nlohmann::ordered_json MapHandler::createOrderedEntityJson(ecs::Entity entity) const
{
    nlohmann::ordered_json entityJson;

    if (registry.has<ecs::SpriteComponent>(entity)) {
        auto sprite = registry.tryGet<ecs::SpriteComponent>(entity);
        if (const auto *texture = sprite.sprite.getTexture()) {
            std::string textureKey = findTextureKey(texture);
            if (!textureKey.empty()) {
                entityJson["texture"] = textureKey;
            }
        }
    }

    if (registry.has<ecs::PositionComponent>(entity)) {
        auto pos = registry.tryGet<ecs::PositionComponent>(entity);
        entityJson["position"] = {pos.x, pos.y};
    }

    if (registry.has<ecs::VelocityComponent>(entity)) {
        auto vel = registry.tryGet<ecs::VelocityComponent>(entity);
        entityJson["velocity"] = {vel.vx, vel.vy};
    }

    if (registry.has<ecs::SpriteComponent>(entity)) {
        auto sprite = registry.tryGet<ecs::SpriteComponent>(entity);
        entityJson["scale"] = {sprite.sprite.getScale().x, sprite.sprite.getScale().y};
    }

    if (registry.has<ecs::AnimationComponent>(entity)) {
        auto anim = registry.tryGet<ecs::AnimationComponent>(entity);
        std::string animKey = findAnimationKey(anim);
        if (!animKey.empty()) {
            entityJson["animation"] = animKey;
        }
    }

    if (registry.has<ecs::HitboxComponent>(entity)) {
        auto hitbox = registry.tryGet<ecs::HitboxComponent>(entity);
        entityJson["hitboxOffset"] = {hitbox.offset.x, hitbox.offset.y};
    }

    return entityJson;
}

// main operations
void MapHandler::load(const std::string &mapPath, MapComponent components)
{
    std::ifstream file(mapPath);
    if (!file.is_open()) {
        LOG_ERROR("failed to open map file: {}", mapPath);
        return;
    }
    try {
        mapData = nlohmann::ordered_json::parse(file);

        if (static_cast<int>(components & MapComponent::BasicInfo))
            loadBasicInfo();

        if (static_cast<int>(components & MapComponent::Assets))
            loadAssets();

        if (static_cast<int>(components & MapComponent::Background))
            loadBackground();

        if (static_cast<int>(components & MapComponent::Player))
            loadPlayer();

        if (static_cast<int>(components & MapComponent::Enemies))
            loadEnemies();
    } catch (const nlohmann::json::parse_error &e) {
        LOG_ERROR("failed to parse map file: {}", e.what());
    }
}

void MapHandler::save(const std::string &mapPath)
{
    try {
        nlohmann::ordered_json orderedMapData;

        orderedMapData["mapWidth"] = map.width;
        orderedMapData["mapHeight"] = map.height;
        if (!map.music.empty()) {
            orderedMapData["music"] = map.music;
        }

        saveAssets(orderedMapData);
        saveBackground(orderedMapData);
        saveMissile(orderedMapData);
        savePlayer(orderedMapData);
        saveEnemies(orderedMapData);

        std::ofstream file(mapPath);
        if (!file.is_open()) {
            LOG_ERROR("failed to open map file for saving: {}", mapPath);
            return;
        }
        file << orderedMapData.dump(2);
    } catch (const std::exception &e) {
        LOG_ERROR("failed to write map file: {}", e.what());
    }
}

// loading methods
void MapHandler::loadBasicInfo()
{
    map.width = mapData.value("mapWidth", 1920.0f);
    map.height = mapData.value("mapHeight", 1080.0f);
    map.music = mapData.value("music", "");
}

void MapHandler::loadAssets()
{
    if (mapData.contains("textures") && mapData["textures"].is_object()) {
        for (const auto &[key, path] : mapData["textures"].items()) {
            Map::TextureData textureData;
            textureData.path = path;
            if (!textureData.texture.loadFromFile(path)) {
                LOG_WARNING("failed to load texture: {}", textureData.path);
            } else {
                map.textures[key] = std::move(textureData);
            }
        }
    }

    if (mapData.contains("animations") && mapData["animations"].is_object()) {
        for (const auto &[key, path] : mapData["animations"].items()) {
            map.animations[key] = path;
        }
    }
}

void MapHandler::loadBackground()
{
    if (!mapData.contains("background") || !mapData["background"].is_array()) {
        LOG_INFO("no background data found in map file");
        return;
    }

    ecs::Entity backgroundEntity = registry.createEntity();
    auto &bgComponent = registry.set<ecs::BackgroundComponent>(backgroundEntity, ecs::BackgroundComponent(map.width));

    for (const auto &element : mapData["background"]) {
        if (!element.is_object()) {
            LOG_WARNING("invalid background element");
            continue;
        }

        std::string type = element.value("type", "");
        std::string texture = element.value("texture", "");
        float scrollSpeed = element.value("scrollSpeed", 0.0f);

        if (type.empty() || texture.empty() || map.textures.find(texture) == map.textures.end()) {
            LOG_WARNING("invalid background element data");
            continue;
        }

        if (type == "regular") {
            bgComponent.addRegularElement(map.textures[texture].texture, scrollSpeed,
                                          {static_cast<unsigned int>(map.width), static_cast<unsigned int>(map.height)});
        } else if (type == "random") {
            int count = element.value("count", 1);
            for (int i = 0; i < count; ++i) {
                bgComponent.addRandomElement(map.textures[texture].texture, scrollSpeed,
                                             {static_cast<unsigned int>(map.width), static_cast<unsigned int>(map.height)});
            }
        }
    }
}

void MapHandler::loadPlayer()
{
    if (!mapData.contains("player") || !mapData["player"].is_object()) {
        LOG_INFO("no player data found in map file");
        return;
    }

    const auto &playerData = mapData["player"];
    PlayerData data;

    data.position = {playerData.value("position", std::vector<float>{0.0f, 0.0f})[0],
                     playerData.value("position", std::vector<float>{0.0f, 0.0f})[1]};
    data.scale = {playerData.value("scale", std::vector<float>{1.0f, 1.0f})[0],
                  playerData.value("scale", std::vector<float>{1.0f, 1.0f})[1]};
    data.maxSpeed = {playerData.value("maxSpeed", std::vector<float>{100.0f, 100.0f})[0],
                     playerData.value("maxSpeed", std::vector<float>{100.0f, 100.0f})[1]};
    data.hitboxOffset = {playerData.value("hitboxOffset", std::vector<float>{0.0f, 0.0f})[0],
                         playerData.value("hitboxOffset", std::vector<float>{0.0f, 0.0f})[1]};
    data.texture = playerData.value("texture", "");
    data.animation = playerData.value("animation", "");
    data.health = playerData.value("health", 100.0f);
    data.tag = playerData.value("tag", "");

    spawnPlayer(data);
}

void MapHandler::loadEnemies()
{
    if (!mapData.contains("enemies") || !mapData["enemies"].is_array()) {
        LOG_INFO("no enemies data found in map file");
        return;
    }

    for (const auto &enemyData : mapData["enemies"]) {
        if (!enemyData.is_object()) {
            LOG_WARNING("invalid enemy data");
            continue;
        }

        EnemyData data;

        data.position = {enemyData.value("position", std::vector<float>{0.0f, 0.0f})[0],
                         enemyData.value("position", std::vector<float>{0.0f, 0.0f})[1]};
        data.velocity = {enemyData.value("velocity", std::vector<float>{0.0f, 0.0f})[0],
                         enemyData.value("velocity", std::vector<float>{0.0f, 0.0f})[1]};
        data.scale = {enemyData.value("scale", std::vector<float>{1.0f, 1.0f})[0],
                      enemyData.value("scale", std::vector<float>{1.0f, 1.0f})[1]};
        data.hitboxOffset = {enemyData.value("hitboxOffset", std::vector<float>{0.0f, 0.0f})[0],
                             enemyData.value("hitboxOffset", std::vector<float>{0.0f, 0.0f})[1]};
        data.texture = enemyData.value("texture", "");
        data.animation = enemyData.value("animation", "");
        data.health = enemyData.value("health", 100);
        data.showHitbox = enemyData.value("showHitbox", false);
        data.tag = enemyData.value("tag", "");

        spawnEnemy(data);
    }
}

// save methods
void MapHandler::saveAssets(nlohmann::ordered_json &mapData)
{
    // Save textures
    nlohmann::ordered_json texturesJson;
    for (const auto &[key, textureData] : map.textures) {
        texturesJson[key] = textureData.path;
    }
    mapData["textures"] = texturesJson;

    // Save animations
    nlohmann::ordered_json animationsJson;
    for (const auto &[key, path] : map.animations) {
        animationsJson[key] = path;
    }
    mapData["animations"] = animationsJson;
}

void MapHandler::saveBackground(nlohmann::ordered_json &mapData)
{
    nlohmann::ordered_json backgroundArray = nlohmann::ordered_json::array();
    auto backgrounds = registry.getAllByComponents<ecs::BackgroundComponent>();

    if (backgrounds.empty())
        return;

    std::unordered_map<ElementKey, int, ElementKeyHash> elementGroups;

    const auto &bgComp = std::get<1>(backgrounds[0]);
    for (const auto &element : bgComp.elements) {
        std::string textureKey = findTextureKey(element.sprite.getTexture());
        ElementKey key{element.isRandom ? "random" : "regular", textureKey, element.scrollSpeed};
        elementGroups[key]++;
    }

    for (const auto &[key, count] : elementGroups) {
        nlohmann::ordered_json elementJson;
        elementJson["type"] = key.type;
        elementJson["texture"] = key.texture;
        elementJson["scrollSpeed"] = key.scrollSpeed;

        if (key.type == "random") {
            elementJson["count"] = count;
        }

        backgroundArray.push_back(elementJson);
    }

    mapData["background"] = backgroundArray;
}

void MapHandler::saveMissile(nlohmann::ordered_json &mapData)
{
    nlohmann::ordered_json missileJson;
    missileJson["texture"] = "missile";
    missileJson["position"] = {0.0f, 0.0f};
    missileJson["maxSpeed"] = {0.0f, 0.0f};
    missileJson["scale"] = {3.0f, 3.0f};
    missileJson["animation"] = "missile";
    missileJson["hitboxOffset"] = {-16.0f, -16.0f};

    mapData["missile"] = missileJson;
}

void MapHandler::savePlayer(nlohmann::ordered_json &mapData)
{
    auto players = registry.getAllByComponents<ecs::PlayerComponent, ecs::PositionComponent, ecs::VelocityComponent, ecs::HealthComponent,
                                               ecs::SpriteComponent, ecs::HitboxComponent>();

    if (players.empty()) {
        mapData["player"] = nlohmann::ordered_json::object();
        return;
    }

    const auto &[entity, playerComp, pos, vel, health, sprite, hitbox] = players[0];
    nlohmann::ordered_json playerJson = createOrderedEntityJson(entity);

    // Add player-specific data
    playerJson["health"] = health.maxHealth;
    playerJson["maxSpeed"] = {vel.sx, vel.sy};

    mapData["player"] = playerJson;
}

void MapHandler::saveEnemies(nlohmann::ordered_json &mapData)
{
    nlohmann::ordered_json enemiesArray = nlohmann::ordered_json::array();

    auto enemies = registry.getAllByComponents<ecs::EnemyComponent, ecs::PositionComponent, ecs::VelocityComponent, ecs::SpriteComponent,
                                               ecs::HitboxComponent>();

    for (const auto &[entity, enemyComp, pos, vel, sprite, hitbox] : enemies) {
        nlohmann::ordered_json enemyJson = createOrderedEntityJson(entity);

        if (!enemyJson.contains("animation") && registry.has<ecs::AnimationComponent>(entity)) {
            auto anim = registry.tryGet<ecs::AnimationComponent>(entity);
            std::string animKey = findAnimationKey(anim);
            if (!animKey.empty()) {
                enemyJson["animation"] = animKey;
            }
        }

        enemiesArray.push_back(enemyJson);
    }

    mapData["enemies"] = enemiesArray;
}

// entity helpers
void MapHandler::addSpriteToPlayer(ecs::Entity player) const
{
    if (!mapData.contains("player") || !mapData["player"].is_object()) {
        LOG_INFO("no player data found in map file");
        return;
    }

    registry.set<ecs::PlayerComponent>(player, ecs::PlayerComponent(fmt::format("Player {}", player)));
    loadEntity(mapData["player"], player);
}

void MapHandler::addSpriteToBullet(ecs::Entity bullet) const
{
    if (map.textures.find("missile") != map.textures.end() && map.animations.find("missile") != map.animations.end()) {
        registry.set<ecs::SpriteComponent>(bullet, ecs::SpriteComponent(map.textures.at("missile").texture, {3.0f, 3.0f}));
        registry.set<ecs::AnimationComponent>(bullet, ecs::AnimationComponent(map.animations.at("missile")));
        registry.set<ecs::HitboxComponent>(bullet,
                                           ecs::HitboxComponent(registry.tryGet<ecs::SpriteComponent>(bullet).sprite, {-18.0f, -10.0f}));
    } else {
        LOG_WARNING("missile assets not found");
    }
}
