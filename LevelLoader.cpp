#pragma once

#include "LevelLoader.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>
#include <memory>
#include "GameObject.hpp"
#include "MAMGame.hpp"
#include "SpriteComponent.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>

using namespace std;
using namespace rapidjson;

void LevelLoader::loadMap(std::string filename) {
    ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);
    rapidjson::Value& layers = d["layers"];
    rapidjson::Value& layerObj = layers[0];
    
    rapidjson::Value& tilesets = d["tilesets"];
    tileHeight = d["tileheight"].GetInt();
    tileWidth = d["tilewidth"].GetInt();
    tileSize = glm::vec2(tileWidth / 2, tileHeight / 2);
    glm::vec2 size = getTileSize() / MAMGame::instance->physicsScale;

    int x = layerObj["x"].GetInt();
    int y = layerObj["y"].GetInt();
    int height = layerObj["height"].GetInt();
    int width = layerObj["width"].GetInt();

    rapidjson::Value& data = layerObj["data"];
    int verticalCounter = 0;
    glm::vec2 startOfBigPos;
    int bigCount = 0;
    bool startedOnBig = false;
    bool leftDiamond = false;
    bool rightDiamond = false;

    for (rapidjson::SizeType i = 0; i < data.Size(); i++) {
        int hozMod = i % width;
        int xPos = hozMod + x;
        int yPos = (verticalCounter + y) * -1;

        glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight);

        int tileId = data[i].GetInt();
        switch (tileId) {
            case 1: {
                playerPoint = position;
                // Adding player later so he is drawn last
                break;
            }
            case 2:
            case 3:
            case 4:
            case 16:
            case 17:
            case 18: {
                auto tile = createGameObject(position, GOType::ground, tileId);
             
                if (!startedOnBig) {
                    startOfBigPos = position;
                    bigCount++;
                    startedOnBig = true;
                } else {
                    bigCount++;
                }

                if (tileId == 2) leftDiamond = true;
                if (tileId == 4) rightDiamond = true;

                int nextId = (i >= data.Size() || (i + 1) % width == 0) ? 0 : data[i + 1].GetInt();
                if (nextId != 3 && nextId != 16 && nextId != 17 && nextId != 18 && nextId != 2 && nextId != 4) {
                    createBig(tile, startOfBigPos, position, bigCount, leftDiamond, rightDiamond, size);

                    startedOnBig = false;
                    leftDiamond = false;
                    rightDiamond = false;
                    bigCount = 0;
                }
                
                break;
            }
            case 5: { // make bottom right triangle
                auto tile = createGameObject(position, GOType::slope, tileId);

                int lowerLeftId = i + width - 1;
                int nexttileid = (lowerLeftId >= data.Size() || (lowerLeftId) % width == 29) ? 0 : data[lowerLeftId].GetInt();
                if (nexttileid != tileId) {
                    std::cout << "bong" << std::endl;
                    createBigSlopeRight(tile, position, tileId, size, i, width, data);
                }

                
                break;
            }
            case 9: { // make bottom left triangle
                auto tile = createGameObject(position, GOType::slope, tileId);
                
                int lowerRightId = i + width + 1;
                int nexttileid = (lowerRightId >= data.Size() || (lowerRightId) % width == 0) ? 0 : data[lowerRightId].GetInt();
                if (nexttileid != tileId) {
                    std::cout << "bing" << std::endl;
                    createBigSlopeLeft(tile, position, tileId, size, i, width, data);
                }
                
                break;
            }
            case 23: {
                position = position + glm::vec2(0, 15);
                auto tile = createGameObject(position, GOType::target, tileId);
                auto phys = tile->addComponent<PhysicsComponent>();
                phys->initTarget(b2_staticBody, tile->getPosition());
                phys->setSensor(true);
                break;
            }
            case 0:
                break;
            default: {
                auto tile = createGameObject(position, GOType::wall, tileId);
                auto phys = tile->addComponent<PhysicsComponent>();
                phys->initBox(b2_staticBody, getTileSize(), tile->getPosition(), 1);
                break;
            }
        }

        if (hozMod == width - 1) {
            verticalCounter++;
        }
    }
    
    MAMGame::instance->createPlayerObject(playerPoint);
}

std::shared_ptr<GameObject> LevelLoader::createGameObject(glm::vec2 position, GOType type, int tileId) {
    auto tile = MAMGame::instance->createGameObject(position, type);
    auto spriteBox = tile->addComponent<SpriteComponent>();
    auto sprite = MAMGame::instance->getSprite(tileId);
    spriteBox->setSprite(sprite);
    return tile;
}

int LevelLoader::getTileWidth() {
    return tileWidth;
}

int LevelLoader::getTileHeight() {
    return tileHeight;
}

glm::vec2 LevelLoader::getTileSize() {
    return tileSize;
}

void LevelLoader::createBig(std::shared_ptr<GameObject> tile, glm::vec2 startOfBigPos, glm::vec2 position, int bigCount, bool leftDiamond, bool rightDiamond, glm::vec2 size) {
    b2Vec2 vertices[6];
    int vertexCount = 4;
    float mult = 0.5;
    if (leftDiamond) {
        vertices[0].Set(-size.x * bigCount, size.y);
        vertices[1].Set(-size.x * bigCount, size.y * mult);
        vertices[2].Set((-size.x * bigCount) + (size.x * 2) - (size.x * mult), -size.y);
        vertexCount++;
    } else {
        vertices[0].Set(-size.x * bigCount, size.y);
        vertices[1].Set(-size.x * bigCount, -size.y);
    }

    if (rightDiamond) {
        vertices[2 + (vertexCount - 4)].Set((size.x * bigCount) - (size.x * 2) + (size.x * mult), -size.y);
        vertices[3 + (vertexCount - 4)].Set(size.x * bigCount, size.y * mult);
        vertices[4 + (vertexCount - 4)].Set(size.x * bigCount, size.y);
        vertexCount++;
    } else {
        vertices[2 + (vertexCount - 4)].Set(size.x * bigCount, -size.y);
        vertices[3 + (vertexCount - 4)].Set(size.x * bigCount, size.y);
    }

    auto phys = tile->addComponent<PhysicsComponent>();
    glm::vec2 center = glm::vec2((startOfBigPos.x + position.x) / 2, position.y);
    phys->initPolygon(b2_staticBody, center, 1, vertices, vertexCount, 1);
}


void LevelLoader::createBigSlopeRight(std::shared_ptr<GameObject> tile, glm::vec2 position, int tileId, glm::vec2 size, int initialIndex, int width, rapidjson::Value& data) {
    
    int slopeCount = 1; 
    int nextCandidate = initialIndex - width + 1;
    int nextId = (nextCandidate < 0 || (nextCandidate) % width == 0) ? 0 : data[nextCandidate].GetInt();
    while (nextId == tileId) {
        slopeCount++;
        nextCandidate = nextCandidate - width + 1;
        nextId = (nextCandidate < 0 || (nextCandidate) % width == 0) ? 0 : data[nextCandidate].GetInt();
    }
    
    std::cout << "slope count " << slopeCount << std::endl;
    
    std::vector<b2Vec2> vertices;

    glm::vec2 otherPosition = glm::vec2(position.x + ((slopeCount-1) * tileWidth), position.y + ((slopeCount-1) * tileHeight));

    glm::vec2 center = glm::vec2((otherPosition.x + position.x) / 2, (otherPosition.y + position.y) / 2);
    
    float offset = slopeCount / 2.0f; 

    for (int i = 0; i < slopeCount; i++) {
        float x = -((size.x * 2) * (offset - i));
        float y = -((size.y * 2) * (offset - i));
        b2Vec2 firstPoint = b2Vec2(x, y);
        b2Vec2 secondPoint = b2Vec2(x + (size.x*2), y);

        vertices.push_back(firstPoint);
        vertices.push_back(secondPoint);

    }
    b2Vec2 lastPoint = b2Vec2((size.x * slopeCount), (size.y * slopeCount));
    vertices.push_back(lastPoint);
    
    
    
    auto phys = tile->addComponent<PhysicsComponent>();

    int vertexCount = (slopeCount * 2) + 1;

    phys->initPolygon(b2_staticBody, center, 1, &vertices[0], vertexCount, 1);
}

void LevelLoader::createBigSlopeLeft(std::shared_ptr<GameObject> tile, glm::vec2 position, int tileId, glm::vec2 size, int initialIndex, int width, rapidjson::Value& data) {

    int slopeCount = 1;
    int nextCandidate = initialIndex - width - 1;
    int nextId = (nextCandidate < 0 || (nextCandidate) % width == 29) ? 0 : data[nextCandidate].GetInt();
    while (nextId == tileId) {
        slopeCount++;
        nextCandidate = nextCandidate - width - 1;
        nextId = (nextCandidate < 0 || (nextCandidate) % width == 29) ? 0 : data[nextCandidate].GetInt();
    }

    std::cout << "slope count " << slopeCount << std::endl;

    std::vector<b2Vec2> vertices;

    glm::vec2 otherPosition = glm::vec2(position.x - ((slopeCount - 1) * tileWidth), position.y + ((slopeCount - 1) * tileHeight));

    glm::vec2 center = glm::vec2((otherPosition.x + position.x) / 2, (otherPosition.y + position.y) / 2);

    float offset = slopeCount / 2.0f;

    for (int i = 0; i < slopeCount; i++) {
        float x = ((size.x * 2) * (offset - i));
        float y = -((size.y * 2) * (offset - i));
        b2Vec2 firstPoint = b2Vec2(x, y);
        b2Vec2 secondPoint = b2Vec2(x - (size.x * 2), y);

        vertices.push_back(firstPoint);
        vertices.push_back(secondPoint);

    }
    b2Vec2 lastPoint = b2Vec2(-(size.x * slopeCount), (size.y * slopeCount));
    vertices.push_back(lastPoint);



    auto phys = tile->addComponent<PhysicsComponent>();

    int vertexCount = (slopeCount * 2) + 1;

    phys->initPolygon(b2_staticBody, center, 1, &vertices[0], vertexCount, 1);
}