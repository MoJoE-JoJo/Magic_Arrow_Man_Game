#pragma once
#include "../../../rapidjson/rapidjson.h"
#include "../../../rapidjson/document.h"
#include "../../../rapidjson/istreamwrapper.h"
#include "../../../rapidjson/ostreamwrapper.h"
#include "../../../rapidjson/writer.h"
#include "sre/Texture.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "Gui.hpp"
#include "../../MAMGame.hpp"

using namespace rapidjson;

Gui::Gui(glm::vec2 windowSize) {
    this->windowSize = windowSize;

    auto fonts = ImGui::GetIO().Fonts;
    fonts->AddFontDefault();
    auto fontName = "Assets/Fonts/Swanky/Swanky.ttf";
    headerFont = fonts->AddFontFromFileTTF(fontName, 50);
    normalFont = fonts->AddFontFromFileTTF(fontName, 25);

    loadProgress();
}

void Gui::loadProgress() {
    tutorialsList.clear();
    levelsList.clear();

    std::ifstream fis("Assets/Levels/Progress.json");
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    Value& tutorials = d["Tutorials"];
    for (int i = 0; i < tutorials.Size(); i++) {
        Value& tutorial = tutorials[i];
        std::string name = tutorial["Name"].GetString();
        std::string file = tutorial["File"].GetString();
        bool completed = tutorial["Completed"].GetBool();

        auto levelObj = std::shared_ptr<Level>(new Level(name, file, "", "", "", "", completed, 0));
        tutorialsList.push_back(levelObj);
    }

    Value& levels = d["Levels"];
    for (int i = 0; i < levels.Size(); i++) {
        Value& level = levels[i];
        std::string name = level["Name"].GetString();
        std::string file = level["File"].GetString();
        bool completed = level["Completed"].GetBool();
        std::string bestTime = level["BestTime"].GetString();
        std::string gold = level["Gold"].GetString();
        std::string silver = level["Silver"].GetString();
        std::string bronze = level["Bronze"].GetString();
        int difficulty = level["Difficulty"].GetInt();

        auto levelObj = std::shared_ptr<Level>(new Level(name, file, bestTime, gold, silver, bronze, completed, difficulty));
        levelsList.push_back(levelObj);
    }
}

void Gui::renderGui(GuiState guiState) {
    if (guiState == GuiState::MainMenu) renderMenu();
    else if (guiState == GuiState::LevelSelect) renderLevelSelect();
    else if (guiState == GuiState::Won) renderWinScreen();
    else if (guiState == GuiState::HowToPlay) renderHowToPlayScreen();
}

void Gui::renderMenu() {
    ImGui::PushFont(headerFont);

    ImVec2 size = { 420, 575 };
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(1);
    ImVec2 pos = { (windowSize.x - size.x) / 2, (windowSize.y - size.y) / 2 };
    ImGui::SetNextWindowPos(pos);

    auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    bool* open = nullptr;
    ImGui::Begin("", open, flags);

    static auto logo = sre::Texture::create().withFile("Assets/Sprites/logo.png").withFilterSampling(false).build();
    auto logoId = logo->getNativeTexturePtr();
    ImVec2 logoSize(400, 300);
    ImGui::Image(logoId, logoSize, ImVec2(0, 1), ImVec2(1, 0));

    ImVec2 buttonSize(300, 60);
    ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - 10 - buttonSize.x) / 2, (ImGui::GetWindowSize().y + 175 - buttonSize.y) / 2 });
    if (ImGui::Button("Play", buttonSize)) {
        MAMGame::instance->setGuiState(GuiState::LevelSelect);
    }

    ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - 10 - buttonSize.x) / 2, (ImGui::GetWindowSize().y + 325 - buttonSize.y) / 2 });
    if (ImGui::Button("Tutorial", buttonSize)) {
        tutorialMode = true;
        MAMGame::instance->setGuiState(GuiState::LevelSelect);
    }

    ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - 10 - buttonSize.x) / 2, (ImGui::GetWindowSize().y + 475 - buttonSize.y) / 2 });
    if (ImGui::Button("How to play", buttonSize)) {
        MAMGame::instance->setGuiState(GuiState::HowToPlay);
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
  
    int size = tutorialMode ? tutorialsList.size() : levelsList.size();
    for (int i = 0; i < size; i++) {
        auto level = tutorialMode ? tutorialsList[i] : levelsList[i];
        bool btnPress = ImGui::Button(level->name.c_str(), buttonSize);
        if (btnPress) {
            MAMGame::instance->beginLevel(level->file);
        }
        std::string hasCompleted = level->completed ? "yes" : "no";
        std::string completed = "Completed: " + hasCompleted;
        ImGui::TextColored(level->completed ? colorWon : colorNotWon, completed.c_str());

        if (!tutorialMode) {
            std::string printBestTime = "Best time: " + level->bestTime;

            if (level->rank > 2) {
                ImGui::Text(printBestTime.c_str());
            } else {
                auto color = level->rank == 0 ? colorGold : (level->rank == 1 ? colorSilver : colorBronze);
                ImGui::TextColored(color, printBestTime.c_str());
            }

            std::string stars = "";
            for (int s = 0; s < level->difficulty; s++) {
                stars = stars + "*";
            }
            std::string diffString = "Difficulty: " + stars;
            ImVec4 diffColor = level->difficulty == 1 ? colorDiff1 : (level->difficulty == 2 ? colorDiff2 : (level->difficulty == 3) ? colorDiff3 : colorDiff5);

            ImGui::TextColored(diffColor, diffString.c_str());
        } else {
            ImGui::Text("");
        }

        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Text("");
    if (ImGui::Button("Back", ImVec2(100, 60))) {
        tutorialMode = false;
        MAMGame::instance->setGuiState(GuiState::MainMenu);
    }

    ImGui::End();

    ImGui::PopFont();
}

void Gui::renderWinScreen() {
    ImGui::PushFont(normalFont);

    ImVec2 size = { 400, 250 };
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(1);
    ImVec2 pos = { (windowSize.x - size.x) / 2, (windowSize.y - size.y) / 2 };
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Text("You won the level!");

    if (!tutorialMode) {
        auto title = std::string("Your time: " + Level::GetString(winTime));
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
    }

    ImGui::Text("");
    ImGui::Columns(2, NULL, false);
    if (ImGui::Button("Ok", { 60, 40 })) {
        MAMGame::instance->setGuiState(GuiState::LevelSelect);
    }
    ImGui::NextColumn();
    if (ImGui::Button("Reset", { 80, 40 })) {
        MAMGame::instance->reset();
    }
    ImGui::Columns(1);
    ImGui::End();

    ImGui::PopFont();
}

void Gui::renderHowToPlayScreen() {
    ImGui::PushFont(normalFont);

    ImVec2 size = { 600, 550 };
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(1);
    ImVec2 pos = { (windowSize.x - size.x) / 2, (windowSize.y - size.y) / 2 };
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Text("You control Magic Arrow Man with the A and D keys,");
    ImGui::Text("reset a level with the R key and quit a level with Q.");
    ImGui::Text("");
    ImGui::Text("He jumps when you press space bar. He can climb ledges if");
    ImGui::Text("you continue to hold the space bar.");
    ImGui::Text("");
    ImGui::Text("When he has the bow and arrow, he shoots the arrow");
    ImGui::Text("when you click with the left mouse button.");
    ImGui::Text("");
    ImGui::Text("If Magic Arrow Man stands still and his arrow is far away,");
    ImGui::Text("he can call it back when you hold the left mouse button.");
    ImGui::Text("");
    ImGui::Text("If Magic Arrow Man is flying in the air and his arrow is");
    ImGui::Text("far away, both him and his arrow will be propelled towards");
    ImGui::Text("eachother. Although he can only do this once before having");
    ImGui::Text("to get back to the ground.");
    ImGui::Text("");
    if (ImGui::Button("Ok", { 60, 40 })) {
        MAMGame::instance->setGuiState(GuiState::MainMenu);
    }
    ImGui::End();

    ImGui::PopFont();
}

void Gui::setWinTime(int seconds, std::string levelFileName) {
    winTime = seconds;
    int levelIndex = 0;

    if (tutorialMode) {
        for (auto& level : tutorialsList) {
            if (level->file == levelFileName) {
                wonLevel = level;
                break;
            }
            levelIndex++;
        }
    } else {
        for (auto& level : levelsList) {
            if (level->file == levelFileName) {
                wonLevel = level;
                break;
            }
            levelIndex++;
        }
    }
    updateProgress(levelIndex);
    loadProgress();
}

void Gui::updateProgress(int levelIndex) {
    std::ifstream fis("Assets/Levels/Progress.json");
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    Value& level = tutorialMode ? d["Tutorials"][levelIndex] :  d["Levels"][levelIndex];

    if (wonLevel->bestTime == "00:00:00" || winTime < Level::GetSeconds(wonLevel->bestTime)) {
        Value newTime;
        char buffer[10];
        int len = sprintf(buffer, Level::GetString(winTime).c_str());
        newTime.SetString(buffer, len, d.GetAllocator());
        memset(buffer, 0, sizeof(buffer));
        level["BestTime"] = newTime;
    }

    level["Completed"].SetBool(true);

    std::ofstream ofs("Assets/Levels/Progress.json");
    OStreamWrapper osw(ofs);
    Writer<OStreamWrapper> writer(osw);
    d.Accept(writer);
}