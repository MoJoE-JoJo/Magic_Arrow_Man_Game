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
    rapidjson::Value& chunkArray = layerObj["chunks"];
    
    rapidjson::Value& tilesets = d["tilesets"];
    tileHeight = d["tileheight"].GetInt();
    tileWidth = d["tilewidth"].GetInt();
    tileSize = glm::vec2(tileWidth / 2, tileHeight / 2);

    for (rapidjson::SizeType i = 0; i < chunkArray.Size(); i++) {
        rapidjson::Value& chunk = chunkArray[i];

        int x = chunk["x"].GetInt();
        int y = chunk["y"].GetInt();
        int height = chunk["height"].GetInt();
        int width = chunk["width"].GetInt();

        rapidjson::Value& data = chunk["data"];
        int horizontalCounter = 0;
        int verticalCounter = 0;
        for (rapidjson::SizeType j = 0; j < data.Size(); j++) {
            int hozMod = horizontalCounter % width;
            int xPos = hozMod + x;
            int yPos = (verticalCounter + y) * -1;

            int tileId = data[j].GetInt();
            switch (tileId) {
                case 1: {
                    glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight);
                    MAMGame::instance->createPlayerObject(position);
                    break;
                }
                case 2:
                case 3:
                case 4:
                case 5:
                case 9:
                case 16:
                case 17:
                case 18: {
                    glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight);
                    auto tile = MAMGame::instance->createGameObject(position, GOType::ground);
                    auto spriteBox = tile->addComponent<SpriteComponent>();
                    auto sprite = MAMGame::instance->getSprite(tileId);
                    spriteBox->setSprite(sprite);
                    auto phys = tile->addComponent<PhysicsComponent>();
                    phys->initBox(b2_staticBody, getTileSize(), tile->getPosition(), 1);
                    break;
                }
                case 0:
                    break;
                default: {
                    glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight);
                    auto tile = MAMGame::instance->createGameObject(position, GOType::wall);
                    auto spriteBox = tile->addComponent<SpriteComponent>();
                    auto sprite = MAMGame::instance->getSprite(tileId);
                    spriteBox->setSprite(sprite);
                    auto phys = tile->addComponent<PhysicsComponent>();
                    phys->initBox(b2_staticBody, getTileSize(), tile->getPosition(), 1);
                    break;
                }
            }

            if (hozMod == width - 1) {
                verticalCounter++;
            }
            horizontalCounter++;
        }
        


    }

    //const rapidjson::Value& spawn = d["spawn"];


    //startingPosition.x = spawn["x"].GetFloat() * -1;
    //startingPosition.y = spawn["y"].GetFloat() * -1;
    //startingRotation = spawn["angle"].GetFloat();

    //floorTile = d["floorTile"].GetInt();
    //ceilTile = d["ceilTile"].GetInt();
    //goalTile = d["goalTile"].GetInt();

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