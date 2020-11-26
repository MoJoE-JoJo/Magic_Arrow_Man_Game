#pragma once
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "Gui.hpp"
#include "MAMGame.hpp"

using namespace rapidjson;

Gui::Gui(glm::vec2 windowSize) {
    this->windowSize = windowSize;

    auto fonts = ImGui::GetIO().Fonts;
    fonts->AddFontDefault();
    auto fontName = "Fonts/Swanky/Swanky.ttf";
    int fontSize = 50;
    font = fonts->AddFontFromFileTTF(fontName, fontSize);
}

void Gui::renderGui() {
    ImGui::PushFont(font);

    if (isRenderMenu) renderMenu();
    else renderLevelSelect();

    ImGui::PopFont();
}

void Gui::renderMenu() {
    ImVec2 size = { 600, 300 };
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(1);
    ImVec2 pos = { (windowSize.x - size.x) / 2, (windowSize.y - size.y) / 2 };
    ImGui::SetNextWindowPos(pos);

    auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    bool* open = nullptr;
    ImGui::Begin("", open, flags);
    auto title = std::string("Magic Arrow Man");
    float font_size = ImGui::GetFontSize() * title.size() / 2;
    ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
    ImGui::TextColored({ 1, 1, 0, 1 }, title.c_str());
    ImVec2 buttonSize(100, 60);
    ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - 10 - buttonSize.x) / 2, (ImGui::GetWindowSize().y - 10 - buttonSize.y) / 2 });
    if (ImGui::Button("Play", buttonSize)) {
        isRenderMenu = false;
    }
    ImGui::End();
}

void Gui::renderLevelSelect() {
    levelFiles.clear();

    std::ifstream fis("Levels/Progress.json");
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    rapidjson::Value& levels = d["Levels"];

    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiSetCond_Always);
    ImGui::Begin("background wall", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Columns(4, NULL);
    ImGui::Separator();
  
    for (int i = 0; i < levels.Size(); i++) {
        if (i > 0 && i % 4 == 0) ImGui::Separator();
        rapidjson::Value& level = levels[i];
        std::string name = level["Name"].GetString();
        std::string file = level["File"].GetString();
        bool btnPress = ImGui::Button(name.c_str(), { ImGui::GetIO().DisplaySize.x / 6 - 10, ImGui::GetIO().DisplaySize.y / 6 - 10 });
        if (btnPress) {
            MAMGame::instance->beginLevel(file);
        }
        bool completed = level["Completed"].GetBool();
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::End();
}