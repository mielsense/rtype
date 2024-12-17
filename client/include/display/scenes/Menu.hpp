/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <network/Types.hpp>
#include <queue>
#include <vector>

#include "Registry.hpp"
#include "display/Scene.hpp"
#include "editor/utils/Utils.hpp"
#include "utils/Waitable.hpp"

enum class MenuState { MAIN, MENU_OPTIONS, FADING, LOBBY_SELECT, CONNECTING };

class Menu final: public Scene {
    public:
        using ConnectToGameCallback = std::function<void(std::string, uint16_t)>;
        enum class MenuAction { QueryServerList, StartSingleplayer, OpenEditor, OpenSettings };

    private:
        struct Assets {
                sf::Texture background;
                sf::Texture logo;
                sf::Font font;
                std::vector<sf::Texture> planets;
        } assets;

        struct UI {
                sf::Sprite background;
                sf::Sprite logo;
                std::vector<sf::Sprite> planets;
                sf::RectangleShape fadeRect;
                sf::Text hoverText;
                sf::Text serverText;
        } ui;

        struct Settings {
                bool isOpen = false;
                int tempFpsLimit = 60;
        } settings;

        struct ShaderOption {
                const char *displayName;
                const char *filename;
        };

        struct Resolution {
                const char *name;
                int width;
                int height;
        };

        std::queue<MenuAction> actionQueue = {};
        std::vector<tetriq::ServerInfo> servers;
        MenuState state;
        MenuState nextState;
        float stateTimer;
        int selectedServer;
        bool serversChanged = false;
        bool isPopupOpen = false;
        ConnectToGameCallback changeScene;

        Waitable refreshServerList;
        static constexpr float FADE_DURATION = 1.0f;
        bool isFading;
        float fadeTimer;

        const std::array<ShaderOption, 6> shaderOptions = {{{"Basic", "dithering.fs"},
                                                            {"Achromatopsie", "achromatopsie.fs"},
                                                            {"Deuteranopie", "deuteranopie.fs"},
                                                            {"Protanopie", "protanopie.fs"},
                                                            {"Tritanopie", "tritanopie.fs"},
                                                            {"No Shader", ""}}};
        const std::array<Resolution, 6> resolutionOptions = {{{"1280x720", 1280, 720},
                                                              {"1366x768", 1366, 768},
                                                              {"1600x900", 1600, 900},
                                                              {"1920x1080", 1920, 1080},
                                                              {"2560x1440", 2560, 1440},
                                                              {"3840x2160", 3840, 2160}}};

        void loadAssets();
        void setupUI();
        void startFade(MenuState newState);
        void updateState(float dt);
        void handleInput(const sf::Event &event);
        void updatePlanetLayout();
        void drawMenuButtons();
        void drawSettings();
        void renderPopups();

    public:
        Menu(sf::RenderTexture &fb, ecs::Registry &registry, ConnectToGameCallback changeScene, SceneChangeCallback changeSceneCallback);
        ~Menu()
        {
            refreshServerList.stop();
        }

        void init() override;
        void events(const sf::Event &event) override;
        void update(float dt, bool hasFocus) override;
        void draw(float dt) override;

        void onChange() override;
        void setServers(const std::vector<tetriq::ServerInfo> &servers);
        std::queue<MenuAction> &getActionQueue();
};

#endif // MENU_HPP
