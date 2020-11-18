#pragma once
#include <vector>
#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"

class GameObject;

class LevelLoader {
public:
    void loadMap(std::string filename);
    int getTileWidth();
    int getTileHeight();
    glm::vec2 getTileSize();
private:
    int tileWidth;
    int tileHeight;
    glm::vec2 tileSize;
    glm::vec2 playerPoint;
};