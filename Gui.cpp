#pragma once
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
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
    headerFont = fonts->AddFontFromFileTTF(fontName, 50);
    normalFont = fonts->AddFontFromFileTTF(fontName, 25);

    loadProgress();
}

void Gui::loadProgress() {
    levelsList.clear();

    std::ifstream fis("Levels/Progress.json");
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    rapidjson::Value& levels = d["Levels"];

    for (int i = 0; i < levels.Size(); i++) {
        rapidjson::Value& level = levels[i];
        std::string name = level["Name"].GetString();
        std::string file = level["File"].GetString();
        bool completed = level["Completed"].GetBool();
        std::string bestTime = level["BestTime"].GetString();
        std::string gold = level["Gold"].GetString();
        std::string silver = level["Silver"].GetString();
        std::string bronze = level["Bronze"].GetString();

        auto levelObj = std::shared_ptr<Level>(new Level(name, file, bestTime, gold, silver, bronze, completed));
        levelsList.push_back(levelObj);
    }
}

void Gui::renderGui(GuiState guiState) {
    if (guiState == GuiState::MainMenu) renderMenu();
    else if (guiState == GuiState::LevelSelect) renderLevelSelect();
    else if (guiState == GuiState::Won) renderWinScreen();
}

void Gui::renderMenu() {
    ImGui::PushFont(headerFont);

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
        MAMGame::instance->setGuiState(GuiState::LevelSelect);
    }
    ImGui::End();

    ImGui::PopFont();
}

void Gui::renderLevelSelect() {
    ImGui::PushFont(normalFont);

    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiSetCond_Always);
    ImGui::Begin("background wall", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Columns(5, NULL, false);

    ImVec2 buttonSize = { 200, 100 };
  
    for (int i = 0; i < levelsList.size(); i++) {
        auto level = levelsList[i];
        bool btnPress = ImGui::Button(level->name.c_str(), buttonSize);
        if (btnPress) {
            MAMGame::instance->beginLevel(level->file);
        }
        std::string hasCompleted = level->completed ? "yes" : "no";
        std::string completed = "Completed: " + hasCompleted;
        ImGui::TextColored(level->completed ? colorWon : colorNotWon, completed.c_str());

        std::string printBestTime = "Best time: " + level->bestTime;

        if (level->rank > 2) {
            ImGui::Text(printBestTime.c_str());
        } else {
            auto color = level->rank == 0 ? colorGold : (level->rank == 1 ? colorSilver : colorBronze);
            ImGui::TextColored(color, printBestTime.c_str());
        }

        ImGui::Text("");
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::End();

    ImGui::PopFont();
}

void Gui::renderWinScreen() {
    ImGui::PushFont(normalFont);

    ImVec2 size = { 400, 240 };
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(1);
    ImVec2 pos = { (windowSize.x - size.x) / 2, (windowSize.y - size.y) / 2 };
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Text("You won the level!");
    auto title = std::string("Your time: " + wonLevel->GetString(winTime));
    ImGui::Text(title.c_str());
    ImGui::Text("");

    if (winTime <= wonLevel->goldTotal) {
        ImGui::Text("You got gold!");
    } else if (winTime <= wonLevel->silverTotal) {
        ImGui::Text("You got silver!");
        auto getGold = std::string("Try again and beat the gold time: " + wonLevel->gold);
        ImGui::Text(getGold.c_str());
    } else if (winTime <= wonLevel->bronzeTotal) {
        ImGui::Text("You got bronze!");
        auto getSilver = std::string("Try again and beat the silver time: " + wonLevel->silver);
        ImGui::Text(getSilver.c_str());
    } else {
        auto getBronze = std::string("Try again and beat the bronze time: " + wonLevel->bronze);
        ImGui::Text(getBronze.c_str());
    }

    ImGui::Text("");
    ImVec2 buttonSize(60, 40);
    if (ImGui::Button("Ok", buttonSize)) {
        MAMGame::instance->setGuiState(GuiState::LevelSelect);
    }
    ImGui::End();

    ImGui::PopFont();
}

void Gui::setWinTime(int seconds, std::string levelFileName) {
    winTime = seconds;
    int levelIndex = 0;
    for (auto& level : levelsList) {
        if (level->file == levelFileName) {
            wonLevel = level;
            break;
        }
        levelIndex++;
    }
    updateProgress(levelIndex);
    loadProgress();
}

void Gui::updateProgress(int levelIndex) {
    std::ifstream fis("Levels/Progress.json");
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    Value& level = d["Levels"][levelIndex];

    if (level["BestTime"] == "00:00:00" || winTime < wonLevel->GetSeconds(wonLevel->bestTime)) {
        Value newTime;
        char buffer[10];
        int len = sprintf(buffer, wonLevel->GetString(winTime).c_str());
        newTime.SetString(buffer, len, d.GetAllocator());
        memset(buffer, 0, sizeof(buffer));
        level["BestTime"] = newTime;
    }

    level["Completed"].SetBool(true);

    std::ofstream ofs("Levels/Progress.json");
    OStreamWrapper osw(ofs);
    Writer<OStreamWrapper> writer(osw);
    d.Accept(writer);
}