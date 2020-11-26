#pragma once
#include <vector>
#include <string>
#include <memory>
#include "sre/SDLRenderer.hpp"

#include "glm/glm.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

class Gui {
public:
	Gui(glm::vec2 windowSize);
	void renderGui();
	void renderMenu();
	void renderLevelSelect();
private:
	glm::vec2 windowSize;
	ImFont* font;
	bool isRenderMenu = true;
};