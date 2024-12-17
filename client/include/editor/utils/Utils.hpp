/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#pragma once

#include <imgui.h>

// imgui utils

#define LIGHT_PURPLE ImVec4(0.8f, 0.6f, 1.0f, 1.0f)

#define IMGUI_IMPORTANT_TEXT(text)                                                                                     \
    ImGui::PushStyleColor(ImGuiCol_Text, LIGHT_PURPLE);                                                                \
    ImGui::Text(text);                                                                                                 \
    ImGui::PopStyleColor();

#define IMGUI_HEADER_TEXT(text)                                                                                        \
    IMGUI_IMPORTANT_TEXT(text)                                                                                         \
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

#define IMGUI_DIVIDER()                                                                                                \
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);                                                                 \
    ImGui::Separator();                                                                                                \
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

#endif // UTILS_HPP
