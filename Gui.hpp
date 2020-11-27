#pragma once
#include <vector>
#include <string>
#include <memory>
#include "sre/SDLRenderer.hpp"
#include "glm/glm.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

#include "Level.hpp"

enum class GuiState { MainMenu, LevelSelect, Won };

class Gui {
public:
	Gui(glm::vec2 windowSize);
	void renderGui(GuiState guiState);
	void loadProgress();
	void setWinTime(int seconds, std::string levelName);
private:
	void renderMenu();
	void renderLevelSelect();
	void renderWinScreen();
	void updateProgress(int levelIndex);

	glm::vec2 windowSize;
	ImFont* headerFont;
	ImFont* normalFont;
	ImVec4 colorGold = { 0.831, 0.686, 0.216, 1 };
	ImVec4 colorSilver = { 0.459, 0.541, 0.612, 1 };
	ImVec4 colorBronze = { 0.576, 0.494, 0.341, 1 };
	ImVec4 colorWon = { 0.141, 0.60, 0.373, 1 };
	ImVec4 colorNotWon = { 0.49, 0.255, 0.22, 1 };
	int winTime = 0;
	std::shared_ptr<Level> wonLevel;
	std::vector<std::shared_ptr<Level>> levelsList;
};