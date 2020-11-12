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

void LevelLoader::loadMap(std::string filename) {
    
    using namespace rapidjson;
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

    for (rapidjson::SizeType i = 0; i < chunkArray.Size(); i++)
    {
        rapidjson::Value& chunk = chunkArray[i];

        int x = chunk["x"].GetInt();
        int y = chunk["y"].GetInt();
        int height = chunk["height"].GetInt();
        int width = chunk["width"].GetInt();
        

        rapidjson::Value& data = chunk["data"];
        int horizontalCounter = 0;
        int verticalCounter = 0;
        for (rapidjson::SizeType j = 0; j < data.Size(); j++) {
            //int tileId = data[j].GetInt();
            int hozMod = horizontalCounter % width;
            int xPos = hozMod + x;
            int yPos = (verticalCounter + y)*-1;
            if (x == 0 && y == 0) {
                std::cout << yPos << endl;
            }
            if (data[j].GetInt() != 0) {

                glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight);
                auto tile = MAMGame::instance->createGameObject(position);
                auto spriteBox = tile->addComponent<SpriteComponent>();
                auto sprite = MAMGame::instance->sprites->get("Tilesets-1-02.png");
                //sprite.setScale({ 30, 30 });
                //sprite.setColor({ 0.89f, 0.55f, 0.0f, 1.0f });
                spriteBox->setSprite(sprite);
                tileObjects.push_back(tile);
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

std::vector<shared_ptr<GameObject>> LevelLoader::getTileObjects() {
    return tileObjects;
}