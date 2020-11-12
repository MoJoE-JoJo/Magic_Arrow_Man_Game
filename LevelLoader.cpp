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
    const rapidjson::Value& layers = d["layers"];
    const rapidjson::Value& layerObj = layers[0];
    const rapidjson::Value& chunkArray = layerObj["chunks"];
    
    const rapidjson::Value& tilesets = d["tilesets"];
    tileHeight = d["tileheight"].GetInt();
    tileWidth = d["tilewidth"].GetInt();

    for (rapidjson::SizeType i = 0; i < chunkArray.Size(); i++)
    {
        const rapidjson::Value& chunk = chunkArray[i];

        int x = chunk["x"].GetInt();
        int y = chunk["y"].GetInt();
        int height = chunk["height"].GetInt();
        int width = chunk["witdth"].GetInt();
        

        const rapidjson::Value& data = chunk["data"];
        int horizontalCounter = 0;
        int verticalCounter = 0;
        for (rapidjson::SizeType j = 0; j < data.Size(); j++) {
            //int tileId = data[j].GetInt();
            int hozMod = horizontalCounter % width;
            if (hozMod == 15) {
                verticalCounter++;
            }
            int xPos = hozMod + x;
            int yPos = verticalCounter + y;

            glm::vec2 position = glm::vec2(xPos*tileWidth, yPos*tileHeight);
            auto tile = make_shared<GameObject>(position);
            auto spriteBox = tile->addComponent<SpriteComponent>();
            auto sprite = MAMGame::instance->sprites->get("sprite");
            sprite.setScale({ 500, 10 });
            sprite.setColor({ 0.89f, 0.55f, 0.0f, 1.0f });
            spriteBox->setSprite(sprite);

            tileObjects.push_back(tile);

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