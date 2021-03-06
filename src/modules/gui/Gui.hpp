#pragma once
#include <vector>
#include <string>
#include <memory>
#include "sre/SDLRenderer.hpp"
#include "glm/glm.hpp"
#include "../../../rapidjson/rapidjson.h"
#include "../../../rapidjson/document.h"

#include "../level_loading/Level.hpp"

enum class GuiState { MainMenu, LevelSelect, Won, HowToPlay };

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
	void renderHowToPlayScreen();
	void updateProgress(int levelIndex);

	glm::vec2 windowSize;
	ImFont* headerFont;
	ImFont* normalFont;
	ImVec4 colorGold = { 0.831, 0.686, 0.216, 1 };
	ImVec4 colorSilver = { 0.459, 0.541, 0.612, 1 };
	ImVec4 colorBronze = { 0.576, 0.494, 0.341, 1 };
	ImVec4 colorWon = { 0.141, 0.60, 0.373, 1 };
	ImVec4 colorNotWon = { 0.49, 0.255, 0.22, 1 };
	ImVec4 colorDiff1 = { 0.149, 0.384, 0.259, 1 };
	ImVec4 colorDiff2 = { 0.941, 0.769, 0.125, 1 };
	ImVec4 colorDiff3 = { 0.933, 0.251, 0.0, 1 };
	ImVec4 colorDiff5 = { 1.0, 0.0, 0.0, 1 };
	bool tutorialMode = false;
	int winTime = 0;
	std::shared_ptr<Level> wonLevel;
	std::vector<std::shared_ptr<Level>> levelsList;
	std::vector<std::shared_ptr<Level>> tutorialsList;
};