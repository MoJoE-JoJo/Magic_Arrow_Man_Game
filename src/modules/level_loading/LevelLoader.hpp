#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../../../rapidjson/rapidjson.h"
#include "../../../rapidjson/document.h"
#include "glm/glm.hpp"

#include "../../GameObject.hpp"

class GameObject;

class LevelLoader {
public:
    void loadMap(std::string filename);
    int getTileWidth();
    int getTileHeight();
    float getMapHeight();
    float getMapWidth();
    glm::vec2 getTileSize();
private:
    int tileWidth;
    int tileHeight;
    glm::vec2 tileSize;
    glm::vec2 playerPoint;
    float mapHeight;
    float mapWidth;

    std::shared_ptr<GameObject> createGameObject(glm::vec2 position, GOType type, int tileId);
    void createBig(std::shared_ptr<GameObject> tile, glm::vec2 startOfBigPos, glm::vec2 position, int bigCount, bool leftDiamond, bool rightDiamond, glm::vec2 size);
    void createBigSlopeRight(std::shared_ptr<GameObject> tile, glm::vec2 position, int tileId, glm::vec2 size, int initialIndex, int width, rapidjson::Value& data);
    void createBigSlopeLeft(std::shared_ptr<GameObject> tile, glm::vec2 position, int tileId, glm::vec2 size, int initialIndex, int width, rapidjson::Value& data);
};