/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef EDITOR_HPP
#define EDITOR_HPP

#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <string>

#include "Registry.hpp"
#include "display/Scene.hpp"
#include "editor/utils/FileBrowser.hpp"
#include "map/MapHandler.hpp"

using SceneChangeCallback = std::function<void(SceneType)>;

class Editor {
    public:
        explicit Editor(sf::Vector2<float> &windowSize, SceneChangeCallback changeSceneCallback);
        ~Editor() = default;

        void render(sf::RenderTexture &frame);
        static void initialize();

        SceneChangeCallback changeScene;

    private:
        void handleMapSelection();
        void loadMap(const std::string &mapPath);
        void createNewMap();
        void renderGameView(sf::RenderTexture &frame);
        void renderDebugControls();
        void renderLoggerPanel();
        void renderMapScrollbar();
        void renderEntities(sf::RenderTexture &frame);
        void updateMapPreview();
        void renderInitialDialog();
        void renderEnemyCreator();
        void saveMap();
        void initEcs();
        void resetState();

        sf::Vector2<float> &windowSize;
        sf::View editorView;
        sf::View initialView;
        float mapWidth;
        ecs::Registry registry;
        MapHandler mapHandler;
        sf::RenderTexture mapPreviewTexture;
        sf::Sprite mapPreviewSprite;
        ImGui::FileBrowser fileDialog;
        std::string currentMapPath;

        char searchBuffer[256] = {0};
        bool autoScroll = true;
        bool autoFocusOnPlayer = false;
        bool shouldFocusOnPlayer = false;

        char saveFileName[256] = {0};
        bool showSaveDialog = false;

        bool shouldChangeScene = false;

        struct EnemyCreationState {
                char selectedTexture[64] = {0};
                char selectedAnimation[64] = {0};
                float positionX = 0.0f;
                float positionY = 0.0f;
                float velocityX = 0.0f;
                float velocityY = 0.0f;
                float scaleX = 1.0f;
                float scaleY = 1.0f;
                float hitboxOffsetX = 0.0f;
                float hitboxOffsetY = 0.0f;
                int health = 100;
                bool showHitbox = false;
        } enemyCreationState;
        bool showEnemyCreator = false;
};

#endif // EDITOR_HPP
