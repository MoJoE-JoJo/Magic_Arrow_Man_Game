#pragma once
#include <vector>
#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "GameObject.hpp"

class GameObject;


class LevelLoader {
public:
    void loadMap(std::string filename);
    int getTileWidth();
    int getTileHeight();
    std::vector<std::shared_ptr<GameObject>> getTileObjects();
private:
    int tileWidth;
    int tileHeight;

    std::vector<std::shared_ptr<GameObject>> tileObjects;
};