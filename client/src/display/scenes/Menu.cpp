/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#include "display/scenes/Menu.hpp"
#include <algorithm>
#include <chrono>
#include <imgui-SFML.h>
#include <imgui.h>
#include <utility>
#include "Logging.hpp"
#include "utils/Data.hpp"

Menu::Menu(sf::RenderTexture &fb, ecs::Registry &registry, ConnectToGameCallback changeScene, SceneChangeCallback changeSceneCallback) :
    Scene(fb, registry, changeSceneCallback), state(MenuState::MAIN), nextState(MenuState::MAIN), stateTimer(0.0f), selectedServer(-1),
    changeScene(std::move(changeScene)),
    refreshServerList(std::chrono::seconds(5), [&] { actionQueue.emplace(MenuAction::QueryServerList); }), isFading(false), fadeTimer(0.0f)
{}

void Menu::loadAssets()
{
    if (!assets.background.loadFromFile("client/assets/textures/menu/background.jpg") ||
        !assets.logo.loadFromFile("client/assets/textures/menu/rtype.png") || !assets.font.loadFromFile("client/assets/font-light.ttf")) {
        LOG(LogLevel::ERROR_LOG, "Failed to load essential assets");
    }

    for (int i = 1; i <= 11; ++i) {
        sf::Texture texture;
        if (texture.loadFromFile("client/assets/textures/menu/planets/planet-" + std::to_string(i) + ".png")) {
            assets.planets.push_back(texture);
        } else {
            LOG(LogLevel::ERROR_LOG, "Failed to load planet texture {}", i);
        }
    }
}

void Menu::setupUI()
{
    sf::Vector2u fbSize = fb.getSize();

    ui.background.setTexture(assets.background);
    ui.logo.setTexture(assets.logo);

    float scale = std::max(static_cast<float>(fbSize.x) / assets.background.getSize().x,
                           static_cast<float>(fbSize.y) / assets.background.getSize().y);
    ui.background.setScale(scale, scale);
    ui.logo.setOrigin(assets.logo.getSize().x / 2.f, assets.logo.getSize().y / 2.f);
    ui.logo.setPosition(fbSize.x / 2.f, fbSize.y / 2.f);

    ui.fadeRect.setSize(sf::Vector2f(fbSize));
    ui.fadeRect.setFillColor(sf::Color::Transparent);

    ui.hoverText.setFont(assets.font);
    ui.hoverText.setCharacterSize(40);
    ui.hoverText.setFillColor(sf::Color::White);
    ui.hoverText.setOutlineColor(sf::Color::Black);
    ui.hoverText.setOutlineThickness(2);

    ui.serverText.setFont(assets.font);
    ui.serverText.setCharacterSize(60);
    ui.serverText.setFillColor(sf::Color::White);
    ui.serverText.setOutlineColor(sf::Color::Black);
    ui.serverText.setOutlineThickness(2);
    ui.serverText.setString("No servers currently available");

    sf::FloatRect textRect = ui.serverText.getLocalBounds();
    ui.serverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    ui.serverText.setPosition(fbSize.x / 2.0f, fbSize.y / 2.0f);

    updatePlanetLayout();
}

void Menu::handleInput(const sf::Event &event)
{
    if (state == MenuState::LOBBY_SELECT) {
        if (isPopupOpen || servers.empty())
            return;

        sf::Vector2f mousePos;
        bool isClick = false;

        if (event.type == sf::Event::MouseMoved) {
            mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
        } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            isClick = true;
        } else {
            return;
        }

        bool hovering = false;
        for (size_t i = 0; i < ui.planets.size(); ++i) {
            if (ui.planets[i].getGlobalBounds().contains(mousePos)) {
                ui.planets[i].setScale(3.5f, 3.5f);
                ui.hoverText.setString(fmt::format("{}\nPlayers: {}/{}", servers[i].name, servers[i].player_count, servers[i].max_players));
                ui.hoverText.setPosition(mousePos.x + 10, mousePos.y + 10);
                hovering = true;

                if (isClick) {
                    selectedServer = i;
                    isPopupOpen = true;
                    ImGui::OpenPopup("Connect to Server");
                }
            } else {
                ui.planets[i].setScale(3.0f, 3.0f);
            }
        }

        if (!hovering) {
            ui.hoverText.setString("");
        }
    }
}

void Menu::updatePlanetLayout()
{
    if (servers.empty()) {
        ui.planets.clear();
        return;
    }

    sf::Vector2u fbSize = fb.getSize();
    float minSize = 100.0f;
    float maxSize = 300.0f;
    float minMargin = 20.0f;

    int maxCols = 5;
    int maxRows = 5;

    int totalPlanets = servers.size();
    int rows, cols;

    if (totalPlanets <= 3) {
        rows = 1;
        cols = totalPlanets;
    } else if (totalPlanets <= 6) {
        rows = 2;
        cols = (totalPlanets + 1) / 2;
    } else {
        rows = std::min(maxRows, (totalPlanets + maxCols - 1) / maxCols);
        cols = std::min(maxCols, (totalPlanets + rows - 1) / rows);
    }

    float availableWidth = fbSize.x - 2 * minMargin;
    float availableHeight = fbSize.y - 2 * minMargin;

    float planetSize = std::min(maxSize, std::max(minSize, std::min(availableWidth / cols, availableHeight / rows)));

    float totalWidth = cols * planetSize;
    float totalHeight = rows * planetSize;

    float horizontalMargin = (availableWidth - totalWidth) / (cols + 1);
    float verticalMargin = (availableHeight - totalHeight) / (rows + 1);

    float startY = (fbSize.y - (rows * planetSize + (rows - 1) * verticalMargin)) / 2;

    ui.planets.clear();
    for (int row = 0; row < rows; ++row) {
        int planetsInThisRow = (row == rows - 1) ? (totalPlanets - row * cols) : cols;
        float rowStartX = (fbSize.x - (planetsInThisRow * planetSize + (planetsInThisRow - 1) * horizontalMargin)) / 2;

        for (int col = 0; col < planetsInThisRow; ++col) {
            size_t i = row * cols + col;
            if (i >= servers.size())
                break;

            float x = rowStartX + col * (planetSize + horizontalMargin);
            float y = startY + row * (planetSize + verticalMargin);

            sf::Sprite sprite(assets.planets[i % assets.planets.size()]);
            sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
            sprite.setPosition(x, y);
            float scale = planetSize / 100.0f;
            sprite.setScale(scale, scale);
            ui.planets.push_back(sprite);
        }
    }
}

void Menu::updateState(float dt)
{
    stateTimer += dt;

    if (isFading) {
        fadeTimer += dt;
        float alpha = 255.0f * (fadeTimer / FADE_DURATION);
        ui.fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));

        if (fadeTimer >= FADE_DURATION) {
            isFading = false;
            fadeTimer = 0.0f;
            state = nextState;
            stateTimer = 0.0f;

            if (state == MenuState::LOBBY_SELECT) {
                actionQueue.emplace(MenuAction::QueryServerList);
            }
        }
        return;
    }

    switch (state) {
        case MenuState::MAIN:
            if (stateTimer >= 2.0f) {
                startFade(MenuState::MENU_OPTIONS);
            }
            break;
        case MenuState::MENU_OPTIONS:
            // wait for user input
            break;
        case MenuState::LOBBY_SELECT:
            for (auto &sprite : ui.planets) {
                int currentFrame = (static_cast<int>(stateTimer * 24) % 30);
                sprite.setTextureRect(sf::IntRect(currentFrame * 100, 0, 100, 100));
            }
            break;
        case MenuState::CONNECTING:
            if (stateTimer >= 1.0f) {
                LOG(LogLevel::INFO, "Connected to server: {}", servers[selectedServer].name);
                startFade(MenuState::MAIN);
            }
            break;
        default:
            break;
    }
}

void Menu::init()
{
    loadAssets();
    setupUI();
    state = MenuState::MAIN;
    stateTimer = 0.0f;
    refreshServerList.start();
}

void Menu::events(const sf::Event &event)
{
    if (!isFading) {
        handleInput(event);
    }
}

void Menu::drawMenuButtons()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(DATA.windowSize.x, DATA.windowSize.y));
    ImGui::Begin("Menu Buttons", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

    float buttonWidth = 250.0f;
    float buttonHeight = 75.0f;
    float buttonSpacing = 20.0f;

    ImVec2 windowSize = ImGui::GetWindowSize();
    float startY = windowSize.y / 2.0f - (buttonHeight * 2 + buttonSpacing) / 2.0f;

    ImGui::SetWindowFontScale(2.0f);

    ImGui::SetCursorPos(ImVec2((windowSize.x - buttonWidth) / 2, startY));
    if (ImGui::Button("Play", ImVec2(buttonWidth, buttonHeight))) {
        startFade(MenuState::LOBBY_SELECT);
    }

    ImGui::SetCursorPos(ImVec2((windowSize.x - buttonWidth) / 2, startY + buttonHeight + buttonSpacing));
    if (ImGui::Button("Editor", ImVec2(buttonWidth, buttonHeight))) {
        DATA.debug = true;
    }

    ImGui::SetWindowFontScale(1.0f);

    ImGui::SetCursorPos(ImVec2(windowSize.x - buttonWidth / 2 - 10, 10));
    if (ImGui::Button("Settings", ImVec2(buttonWidth / 2, buttonHeight / 2))) {
        settings.isOpen = true;
    }

    ImGui::End();
}

void Menu::drawSettings()
{
    float windowWidth = DATA.windowSize.x * 0.4f;
    float windowHeight = DATA.windowSize.y * 0.3f;

    ImGui::SetNextWindowPos(ImVec2(DATA.windowSize.x / 2, DATA.windowSize.y / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    ImGui::OpenPopup("Settings");
    if (ImGui::BeginPopupModal("Settings", &settings.isOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        float contentWidth = windowWidth * 0.6f;

        ImGui::BeginTabBar("SettingsTabs");

        if (ImGui::BeginTabItem("Performance")) {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);

            // fps limit
            ImGui::Text("FPS Limit");
            ImGui::SameLine();
            ImGui::PushItemWidth(contentWidth);
            ImGui::SliderInt("##FPSLimit", &DATA.fpsLimit, 0, 500, DATA.fpsLimit == 0 ? "Unlimited" : "%d FPS");
            ImGui::PopItemWidth();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Video/Accessibility")) {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);

            // resolution
            ImGui::Text("Resolution");
            ImGui::SameLine();
            ImGui::PushItemWidth(contentWidth);

            std::string currentRes =
                std::to_string(static_cast<int>(DATA.targetWindowSize.x)) + "x" + std::to_string(static_cast<int>(DATA.targetWindowSize.y));

            if (ImGui::BeginCombo("##Resolution", currentRes.c_str())) {
                for (const auto &res : resolutionOptions) {
                    bool is_selected = (DATA.targetWindowSize.x == res.width && DATA.targetWindowSize.y == res.height);
                    if (ImGui::Selectable(res.name, is_selected)) {
                        DATA.targetWindowSize = sf::Vector2f(res.width, res.height);
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();

            // shader
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
            ImGui::Text("Shader");
            ImGui::SameLine();
            ImGui::PushItemWidth(contentWidth);

            const char *currentDisplayName = "Basic";
            for (const auto &option : shaderOptions) {
                if (DATA.shader == option.filename) {
                    currentDisplayName = option.displayName;
                    break;
                }
            }

            if (ImGui::BeginCombo("##Shader", currentDisplayName)) {
                for (const auto &option : shaderOptions) {
                    bool is_selected = (DATA.shader == option.filename);
                    if (ImGui::Selectable(option.displayName, is_selected)) {
                        DATA.shader = option.filename;
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Audio")) {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);

            // music
            ImGui::Text("Music");
            ImGui::SameLine();
            ImGui::Checkbox("##ToggleMusic", &DATA.toggleMusic);

            // volume
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
            ImGui::Text("Volume");
            ImGui::SameLine();
            ImGui::PushItemWidth(contentWidth);
            ImGui::SliderFloat("##MusicVolume", &DATA.musicVolume, 0.0f, 100.0f, "%.0f%%");
            ImGui::PopItemWidth();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();

        float buttonWidth = contentWidth * 0.3f;
        float buttonHeight = 30.0f;
        ImGui::SetCursorPos(ImVec2((windowWidth - buttonWidth) / 2, windowHeight - buttonHeight - 10.0f));

        if (ImGui::Button("Save", ImVec2(buttonWidth, buttonHeight))) {
            settings.isOpen = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Menu::renderPopups()
{
    // Settings popup
    if (settings.isOpen) {
        drawSettings();
    }

    // Server connection popup
    static bool popupInitialized = false;
    if (ImGui::BeginPopupModal("Connect to Server", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Do you want to connect to %s?", servers[selectedServer].name.c_str());

        IMGUI_DIVIDER()

        // store player name input
        static char playerNameBuffer[32];
        if (!popupInitialized) {
            strncpy(playerNameBuffer, DATA.playerName.c_str(), sizeof(playerNameBuffer) - 1);
            playerNameBuffer[sizeof(playerNameBuffer) - 1] = '\0';
            popupInitialized = true;
        }

        ImGui::Text("Player Name:");
        ImGui::InputText("##playerName", playerNameBuffer, IM_ARRAYSIZE(playerNameBuffer));

        IMGUI_DIVIDER()

        if (ImGui::Button("Connect", ImVec2(120, 0))) {
            LOG(LogLevel::INFO, "Connecting to server: {} ({}:{}) as {}", servers[selectedServer].name, servers[selectedServer].address,
                servers[selectedServer].port, DATA.playerName);
            DATA.playerName = std::string(playerNameBuffer);
            state = MenuState::CONNECTING;
            isPopupOpen = false;
            popupInitialized = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            isPopupOpen = false;
            popupInitialized = false;
        }
        ImGui::EndPopup();
    } else {
        isPopupOpen = false;
    }
}

void Menu::update(float dt, bool)
{
    updateState(dt);
    refreshServerList.update();

    if (serversChanged) {
        updatePlanetLayout();
        serversChanged = false;
    }

    // Handle refresh button in lobby select state
    if (state == MenuState::LOBBY_SELECT) {
        ImVec2 windowSize = ImGui::GetIO().DisplaySize;
        float buttonWidth = 100.0f;
        float buttonHeight = 30.0f;
        float padding = 10.0f;

        ImVec2 backButtonPos(windowSize.x - (buttonWidth * 2) - (padding * 2), padding);

        ImGui::SetNextWindowPos(ImVec2(backButtonPos.x, backButtonPos.y));
        ImGui::SetNextWindowSize(ImVec2(buttonWidth * 2 + padding, buttonHeight));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

        ImGui::Begin("MenuButtons", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);

        // Back button
        ImGui::SetCursorPos(ImVec2(0, 0));
        if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight))) {
            startFade(MenuState::MENU_OPTIONS);
        }

        // Refresh button
        ImGui::SetCursorPos(ImVec2(buttonWidth + padding, 0));
        if (ImGui::Button("Refresh", ImVec2(buttonWidth, buttonHeight))) {
            actionQueue.emplace(MenuAction::QueryServerList);
        }

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

    // Render all popups in one place
    renderPopups();

    if (state == MenuState::CONNECTING) {
        changeScene(servers[selectedServer].address, servers[selectedServer].port);
        state = MenuState::MAIN;
    }
}

void Menu::startFade(MenuState newState)
{
    isFading = true;
    fadeTimer = 0.0f;
    nextState = newState;
}

void Menu::draw(float dt)
{
    (void) dt;
    fb.draw(ui.background);

    switch (state) {
        case MenuState::MAIN:
            fb.draw(ui.logo);
            break;
        case MenuState::MENU_OPTIONS:
            if (!isFading)
                drawMenuButtons();
            break;
        case MenuState::LOBBY_SELECT:
            if (servers.empty()) {
                fb.draw(ui.serverText);
            } else {
                for (const auto &sprite : ui.planets) {
                    fb.draw(sprite);
                }
                fb.draw(ui.hoverText);
            }
            break;
        case MenuState::CONNECTING:
            break;
        default:
            break;
    }

    if (isFading) {
        fb.draw(ui.fadeRect);
    }
}

void Menu::onChange()
{}

void Menu::setServers(const std::vector<tetriq::ServerInfo> &servers)
{
    this->servers = servers;
    serversChanged = true;
}

std::queue<Menu::MenuAction> &Menu::getActionQueue()
{
    return actionQueue;
}
