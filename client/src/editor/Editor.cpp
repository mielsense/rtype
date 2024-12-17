/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#include "editor/Editor.hpp"
#include "Logging.hpp"
#include "components.hpp"
#include "editor/utils/Utils.hpp"
#include "systems.hpp"
#include "utils/Data.hpp"

Editor::Editor(sf::Vector2<float> &windowSize, SceneChangeCallback changeSceneCallback) :
    changeScene(std::move(changeSceneCallback)),
    windowSize(windowSize), mapWidth(0), mapHandler(registry),
    fileDialog(ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_CreateNewDir)
{
    editorView.setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    editorView.setCenter(windowSize.x / 2.f, windowSize.y / 2.f);
    initialView = editorView;

    fileDialog.SetTitle("Select Map File");
    fileDialog.SetTypeFilters({".json"});
    fileDialog.SetDirectory("./client/assets/maps");

    memset(saveFileName, 0, sizeof(saveFileName));
    initEcs();
}

void Editor::initEcs()
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

void Editor::initialize()
{
    // Imgui style setup
    ImGuiStyle &style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 1.0f;
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowMinSize = ImVec2(20.0f, 20.0f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
    style.FrameRounding = 11.89999961853027f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(4.300000190734863f, 5.5f);
    style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
    style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
    style.IndentSpacing = 0.0f;
    style.ColumnsMinSpacing = 4.900000095367432f;
    style.ScrollbarSize = 11.60000038146973f;
    style.ScrollbarRounding = 15.89999961853027f;
    style.GrabMinSize = 3.700000047683716f;
    style.GrabRounding = 20.0f;
    style.TabRounding = 0.0f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09250493347644806f, 0.100297249853611f, 0.1158798336982727f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1120669096708298f, 0.1262156516313553f, 0.1545064449310303f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.971993625164032f, 1.0f, 0.4980392456054688f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7953379154205322f, 0.4980392456054688f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1821731775999069f, 0.1897992044687271f, 0.1974248886108398f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1545050293207169f, 0.1545048952102661f, 0.1545064449310303f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.1414651423692703f, 0.1629818230867386f, 0.2060086131095886f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1072951927781105f, 0.107295036315918f, 0.1072961091995239f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.1293079704046249f, 0.1479243338108063f, 0.1931330561637878f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1459212601184845f, 0.1459220051765442f, 0.1459227204322815f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9725490212440491f, 1.0f, 0.4980392158031464f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.999999463558197f, 1.0f, 0.9999899864196777f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1249424293637276f, 0.2735691666603088f, 0.5708154439926147f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8841201663017273f, 0.7941429018974304f, 0.5615870356559753f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9570815563201904f, 0.9570719599723816f, 0.9570761322975159f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9356134533882141f, 0.9356129765510559f, 0.9356223344802856f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.266094446182251f, 0.2890366911888123f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
}

void Editor::render(sf::RenderTexture &frame)
{
    if (shouldChangeScene) {
        frame.setView(initialView);
        resetState();
        shouldChangeScene = false;
        changeScene(SceneType::MENU);
        return;
    }

    if (mapWidth == 0) {
        frame.setView(frame.getDefaultView());
        renderInitialDialog();
    } else {
        if (autoFocusOnPlayer || shouldFocusOnPlayer) {
            editorView.setCenter(DATA.cameraCenter.x, editorView.getCenter().y);
            shouldFocusOnPlayer = false;
        }

        updateMapPreview();
        renderGameView(frame);
        renderMapScrollbar();
        renderDebugControls();
        renderLoggerPanel();
        handleMapSelection();

        if (showEnemyCreator) {
            renderEnemyCreator();
        }

        if (showSaveDialog) {
            ImGui::OpenPopup("Save Map");
            ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("Save Map", &showSaveDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Enter map name:");
                ImGui::InputText("##filename", saveFileName, sizeof(saveFileName));

                ImGui::Spacing();
                if (ImGui::Button("Save", ImVec2(120, 0))) {
                    std::string fullPath = "client/assets/maps/" + std::string(saveFileName) + ".json";
                    LOG_INFO("Saving map to: {}", fullPath);
                    mapHandler.save(fullPath);
                    currentMapPath = fullPath;
                    showSaveDialog = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                    showSaveDialog = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
    }

    fileDialog.Display();
    if (fileDialog.HasSelected()) {
        currentMapPath = fileDialog.GetSelected().string();
        loadMap(currentMapPath);
        fileDialog.ClearSelected();
    }
}

void Editor::renderInitialDialog()
{
    ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.35f, windowSize.y * 0.35f), ImGuiCond_Always);

    ImGui::Begin("Map Selection", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 buttonSize(200, 30);
    float totalHeight = buttonSize.y * 2 + ImGui::GetStyle().ItemSpacing.y;

    ImGui::SetCursorPosY((windowSize.y - totalHeight) * 0.5f);

    ImGui::SetCursorPosX((windowSize.x - buttonSize.x) * 0.5f);
    if (ImGui::Button("Load Existing Map", buttonSize)) {
        fileDialog.Open();
    }

    ImGui::SetCursorPosX((windowSize.x - buttonSize.x) * 0.5f);
    if (ImGui::Button("Create New Map", buttonSize)) {
        createNewMap();
    }

    ImGui::SetCursorPosX((windowSize.x - buttonSize.x) * 0.5f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + windowSize.y * 0.1f);
    if (ImGui::Button("Back", buttonSize)) {
        shouldChangeScene = true;
    }

    ImGui::End();
}

void Editor::handleMapSelection()
{
    if (ImGui::Button("Load Map")) {
        fileDialog.Open();
    }

    ImGui::SameLine();

    if (ImGui::Button("Create New Map")) {
        createNewMap();
    }
}

void Editor::loadMap(const std::string &mapPath)
{
    mapHandler.load(mapPath);
    mapWidth = mapHandler.map.width;
    mapPreviewTexture.create(mapWidth, windowSize.y);

    // animation system one time so sprites render (and not spritesheets)
    registry.forEach<ecs::AnimationComponent, ecs::SpriteComponent>([](ecs::AnimationComponent &anim, ecs::SpriteComponent &sprite)
                                                                    { ecs::animationSystem(anim, sprite, 0.00f); });
    updateMapPreview();
    LOG_INFO("Map loaded: {}", mapPath);
}

void Editor::createNewMap()
{
    LOG_INFO("Creating new map...");
    mapWidth = 10000;
    mapHandler.map.width = mapWidth;
    mapPreviewTexture.create(mapWidth, windowSize.y);
    updateMapPreview();
}

void Editor::saveMap()
{
    if (!currentMapPath.empty()) {
        size_t lastSlash = currentMapPath.find_last_of("/\\");
        size_t lastDot = currentMapPath.find_last_of(".");
        std::string filename;
        if (lastSlash != std::string::npos) {
            filename = currentMapPath.substr(lastSlash + 1, lastDot - lastSlash - 1);
        } else {
            filename = currentMapPath.substr(0, lastDot);
        }
        strncpy(saveFileName, filename.c_str(), sizeof(saveFileName) - 1);
    }

    showSaveDialog = true;
}

void Editor::renderGameView(sf::RenderTexture &frame)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.70f, windowSize.y * 0.70f), ImGuiCond_Always);
    ImGui::Begin("Game View", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    float aspectRatio = windowSize.x / windowSize.y;
    ImVec2 previewSize = ImGui::GetWindowSize();
    float width = previewSize.x;
    float height = width / aspectRatio;
    if (height > previewSize.y) {
        height = previewSize.y;
        width = height * aspectRatio;
    }

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY()));
    renderEntities(frame);

    ImGui::Image(frame, sf::Vector2f(width, height));
    ImGui::End();
}

void Editor::renderDebugControls()
{
    ImGui::SetNextWindowPos(ImVec2((float) windowSize.x * 0.70f, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float) windowSize.x * 0.30f, (float) windowSize.y * 0.85f), ImGuiCond_Always);
    ImGui::Begin("Debug Controls", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    float buttonWidth = 80;
    float buttonHeight = 30;
    float rightMargin = 10;

    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - buttonWidth - rightMargin, 10));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

    if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight))) {
        shouldChangeScene = true;
    }
    ImGui::PopStyleColor(3);

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 40));
    IMGUI_HEADER_TEXT("window info")

    ImGui::Text("Width: %f px", windowSize.x);
    ImGui::Text("Height: %f px", windowSize.y);
    ImGui::Text("Current FrameRate: %d fps", static_cast<int>(DATA.fps));

    IMGUI_DIVIDER()
    IMGUI_HEADER_TEXT("editor settings")

    if (ImGui::Checkbox("Auto-focus on Player", &autoFocusOnPlayer)) {
        if (autoFocusOnPlayer) {
            shouldFocusOnPlayer = true;
        }
    }

    if (ImGui::Button("Focus on Player")) {
        shouldFocusOnPlayer = true;
    }

    IMGUI_DIVIDER()
    IMGUI_HEADER_TEXT("enemy creation")

    if (ImGui::Button("Add Enemy")) {
        showEnemyCreator = !showEnemyCreator;
    }

    IMGUI_DIVIDER()
    IMGUI_HEADER_TEXT("map settings")

    if (ImGui::Button("Save map")) {
        saveMap();
    }

    ImGui::End();
}

void Editor::renderLoggerPanel()
{
    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y * 0.85f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y * 0.17f), ImGuiCond_Always);
    ImGui::Begin("Logger", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImGui::Text("Logs:");
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::SameLine();
    ImGui::InputText("Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        Logging::getInstance().clearLogs();
    }

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);

    const auto &logs = Logging::getInstance().getLogs();
    for (const auto &log : logs) {
        if (strlen(searchBuffer) == 0 || log.find(searchBuffer) != std::string::npos) {
            if (log.find("[ERROR]") != std::string::npos) {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            } else if (log.find("[WARNING]") != std::string::npos) {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
            } else if (log.find("[DEBUG]") != std::string::npos) {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(173, 216, 230, 255));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            }
            ImGui::TextWrapped("%s", log.c_str());
            ImGui::PopStyleColor();
        }
    }

    if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();

    ImGui::End();
}

void Editor::renderMapScrollbar()
{
    float gameViewWidth = windowSize.x * 0.70f;
    float mapPreviewHeight = windowSize.y * 0.15f;
    float leftMargin = windowSize.x * 0.01f;
    float rightMargin = windowSize.x * 0.01f;

    ImGui::SetNextWindowPos(ImVec2(0, windowSize.y * 0.70f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(gameViewWidth, mapPreviewHeight), ImGuiCond_Always);
    ImGui::Begin("Map Preview", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    float previewHeight = ImGui::GetContentRegionAvail().y * 0.7f;
    float previewWidth = gameViewWidth - (leftMargin + rightMargin);

    float startPosX = leftMargin;
    ImGui::SetCursorPosX(startPosX);

    ImGui::Image(mapPreviewSprite, ImVec2(previewWidth, previewHeight));

    float visiblePortion = windowSize.x / mapWidth;
    float scrollbarWidth = previewWidth * visiblePortion;

    float maxScroll = previewWidth - scrollbarWidth;
    float scrollPosition = (editorView.getCenter().x - windowSize.x / 2) / (mapWidth - windowSize.x) * maxScroll;
    scrollPosition = std::max(0.0f, std::min(scrollPosition, maxScroll));

    ImVec2 rectMin = ImGui::GetItemRectMin();
    rectMin.x += scrollPosition;
    ImVec2 rectMax = ImVec2(rectMin.x + scrollbarWidth, rectMin.y + previewHeight);
    ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.3f)));

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
    ImGui::SetCursorPosX(startPosX);
    ImGui::PushItemWidth(previewWidth);
    float scrollbarValue = (editorView.getCenter().x - windowSize.x / 2) / (mapWidth - windowSize.x);
    if (ImGui::SliderFloat("##MapScrollbar", &scrollbarValue, 0.0f, 1.0f, "", ImGuiSliderFlags_NoInput)) {
        float newCenterX = scrollbarValue * (mapWidth - windowSize.x) + windowSize.x / 2;
        editorView.setCenter(newCenterX, editorView.getCenter().y);
    }
    ImGui::PopItemWidth();

    ImGui::End();
}

void Editor::renderEntities(sf::RenderTexture &frame)
{
    frame.clear(sf::Color::Black);
    frame.setView(editorView);

    registry.forEach<ecs::PositionComponent, ecs::SpriteComponent, ecs::HitboxComponent>(
        [&](ecs::PositionComponent &pos, ecs::SpriteComponent &sprite, ecs::HitboxComponent &)
        {
            sprite.sprite.setPosition(pos.x, pos.y);
            frame.draw(sprite.sprite);

            sf::RectangleShape hitboxShape;
            hitboxShape.setPosition(pos.x, pos.y);
            hitboxShape.setSize(sf::Vector2f(sprite.sprite.getGlobalBounds().width, sprite.sprite.getGlobalBounds().height));
            hitboxShape.setFillColor(sf::Color::Transparent);
            hitboxShape.setOutlineColor(sf::Color::Magenta);
            hitboxShape.setOutlineThickness(2.0f);
            frame.draw(hitboxShape);
        });

    frame.display();
}

void Editor::updateMapPreview()
{
    mapPreviewTexture.clear(sf::Color::Black);

    sf::View fullMapView(sf::FloatRect(0, 0, mapWidth, windowSize.y));
    mapPreviewTexture.setView(fullMapView);

    registry.forEach<ecs::PositionComponent, ecs::SpriteComponent>(
        [&](ecs::PositionComponent &pos, ecs::SpriteComponent &sprite)
        {
            sprite.sprite.setPosition(pos.x, pos.y);
            mapPreviewTexture.draw(sprite.sprite);
        });

    mapPreviewTexture.display();
    mapPreviewSprite.setTexture(mapPreviewTexture.getTexture());
}

void Editor::renderEnemyCreator()
{
    ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Enemy Creator", &showEnemyCreator)) {
        bool createEnemy = false;

        // Texture Selection
        if (ImGui::BeginCombo("Texture", enemyCreationState.selectedTexture)) {
            for (const auto &[key, texture] : mapHandler.map.textures) {
                bool isSelected = (strcmp(enemyCreationState.selectedTexture, key.c_str()) == 0);
                if (ImGui::Selectable(key.c_str(), isSelected)) {
                    strncpy(enemyCreationState.selectedTexture, key.c_str(), sizeof(enemyCreationState.selectedTexture) - 1);
                }
            }
            ImGui::EndCombo();
        }

        // Animation Selection
        if (ImGui::BeginCombo("Animation", enemyCreationState.selectedAnimation)) {
            for (const auto &[key, animation] : mapHandler.map.animations) {
                bool isSelected = (strcmp(enemyCreationState.selectedAnimation, key.c_str()) == 0);
                if (ImGui::Selectable(key.c_str(), isSelected)) {
                    strncpy(enemyCreationState.selectedAnimation, key.c_str(), sizeof(enemyCreationState.selectedAnimation) - 1);
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        // Position controls
        IMGUI_HEADER_TEXT("position")
        ImGui::DragFloat("X Position", &enemyCreationState.positionX, 1.0f, 0.0f, mapWidth, "%.0f");
        ImGui::DragFloat("Y Position", &enemyCreationState.positionY, 1.0f, 0.0f, windowSize.y, "%.0f");
        if (ImGui::Button("Randomize Position")) {
            enemyCreationState.positionX = static_cast<float>(rand() % static_cast<int>(mapWidth));
            enemyCreationState.positionY = static_cast<float>(rand() % static_cast<int>(windowSize.y));
        }

        ImGui::Separator();

        // Velocity controls
        IMGUI_HEADER_TEXT("velocity")
        ImGui::DragFloat("X Velocity", &enemyCreationState.velocityX, 0.1f, -500.0f, 500.0f, "%.1f");
        ImGui::DragFloat("Y Velocity", &enemyCreationState.velocityY, 0.1f, -500.0f, 500.0f, "%.1f");

        ImGui::Separator();

        // Scale controls
        IMGUI_HEADER_TEXT("scale")
        ImGui::DragFloat("X Scale", &enemyCreationState.scaleX, 0.1f, 0.1f, 10.0f, "%.1f");
        ImGui::DragFloat("Y Scale", &enemyCreationState.scaleY, 0.1f, 0.1f, 10.0f, "%.1f");

        ImGui::Separator();

        // Hitbox controls
        IMGUI_HEADER_TEXT("hitbox")
        ImGui::DragFloat("X Hitbox Offset", &enemyCreationState.hitboxOffsetX, 0.1f, -100.0f, 100.0f, "%.1f");
        ImGui::DragFloat("Y Hitbox Offset", &enemyCreationState.hitboxOffsetY, 0.1f, -100.0f, 100.0f, "%.1f");
        ImGui::Checkbox("Show Hitbox", &enemyCreationState.showHitbox);

        ImGui::Separator();

        // Health control
        IMGUI_HEADER_TEXT("health")
        ImGui::DragInt("Health Points", &enemyCreationState.health, 1.0f, 1, 1000, "%d HP");

        ImGui::Separator();

        ImGui::Spacing();
        ImGui::Spacing();
        if (ImGui::Button("Create Enemy", ImVec2(-1, 40))) {
            createEnemy = true;
        }

        if (createEnemy) {
            MapHandler::EnemyData enemyData;
            enemyData.position = {enemyCreationState.positionX, enemyCreationState.positionY};
            enemyData.velocity = {enemyCreationState.velocityX, enemyCreationState.velocityY};
            enemyData.scale = {enemyCreationState.scaleX, enemyCreationState.scaleY};
            enemyData.hitboxOffset = {enemyCreationState.hitboxOffsetX, enemyCreationState.hitboxOffsetY};
            enemyData.texture = enemyCreationState.selectedTexture;
            enemyData.animation = enemyCreationState.selectedAnimation;
            enemyData.health = enemyCreationState.health;
            enemyData.showHitbox = enemyCreationState.showHitbox;

            mapHandler.spawnEnemy(enemyData);
            // animation system one time so sprites render (and not spritesheets)
            registry.forEach<ecs::AnimationComponent, ecs::SpriteComponent>([](ecs::AnimationComponent &anim, ecs::SpriteComponent &sprite)
                                                                            { ecs::animationSystem(anim, sprite, 0.00f); });

            // Reset to default values
            enemyCreationState.positionX = 0.0f;
            enemyCreationState.positionY = 0.0f;
            enemyCreationState.velocityX = 0.0f;
            enemyCreationState.velocityY = 0.0f;
            enemyCreationState.scaleX = 1.0f;
            enemyCreationState.scaleY = 1.0f;
            enemyCreationState.hitboxOffsetX = 0.0f;
            enemyCreationState.hitboxOffsetY = 0.0f;
            enemyCreationState.health = 100;
            enemyCreationState.showHitbox = false;
        }

        ImGui::End();
    }
}

void Editor::resetState()
{
    //    mapWidth = 0;
    //    currentMapPath.clear();
    //
    //    editorView = initialView;
    //
    //    registry.clear();
    //    initEcs();
    //
    //    memset(searchBuffer, 0, sizeof(searchBuffer));
    //    memset(saveFileName, 0, sizeof(saveFileName));
    //
    //    autoScroll = true;
    //    autoFocusOnPlayer = true;
    //    shouldFocusOnPlayer = false;
    //    showSaveDialog = false;
    //    showEnemyCreator = false;
    //
    //    memset(&enemyCreationState, 0, sizeof(enemyCreationState));
    //    enemyCreationState.scaleX = 1.0f;
    //    enemyCreationState.scaleY = 1.0f;
    //    enemyCreationState.health = 100;
    //
    //    if (mapPreviewTexture.getSize().x > 0) {
    //        mapPreviewTexture.clear();
    //        mapPreviewTexture.create(1, 1);
    //    }
}
