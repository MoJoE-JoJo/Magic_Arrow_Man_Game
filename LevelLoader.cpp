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
                case 3:
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
                case 2: { // make top left diamond
                    glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight);
                    auto tile = MAMGame::instance->createGameObject(position, GOType::ground);
                    auto spriteBox = tile->addComponent<SpriteComponent>();
                    auto sprite = MAMGame::instance->getSprite(tileId);
                    spriteBox->setSprite(sprite);
                    auto phys = tile->addComponent<PhysicsComponent>();
                    b2Vec2 vertices[5];
                    glm::vec2 size = getTileSize() / MAMGame::instance->physicsScale;
                    float mult = 0.5;
                    vertices[0].Set(-size.x, size.y);
                    vertices[1].Set(-size.x, size.y * mult);
                    vertices[2].Set(size.x * mult, -size.y);
                    vertices[3].Set(size.x, -size.y);
                    vertices[4].Set(size.x, size.y);
                    phys->initPolygon(b2_staticBody, tile->getPosition(), 1, vertices, 5);
                    break;
                }
                case 4: { // make top right diamond
                    glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight);
                    auto tile = MAMGame::instance->createGameObject(position, GOType::ground);
                    auto spriteBox = tile->addComponent<SpriteComponent>();
                    auto sprite = MAMGame::instance->getSprite(tileId);
                    spriteBox->setSprite(sprite);
                    auto phys = tile->addComponent<PhysicsComponent>();
                    b2Vec2 vertices[5];
                    glm::vec2 size = getTileSize() / MAMGame::instance->physicsScale;
                    float mult = 0.5;
                    vertices[0].Set(-size.x, size.y);
                    vertices[1].Set(-size.x, -size.y);
                    vertices[2].Set(-size.x * mult, -size.y);
                    vertices[3].Set(size.x, size.y * mult);
                    vertices[4].Set(size.x, size.y);
                    phys->initPolygon(b2_staticBody, tile->getPosition(), 1, vertices, 5);
                    break;
                }
                case 5: { // make bottom right triangle
                    glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight);
                    auto tile = MAMGame::instance->createGameObject(position, GOType::slope);
                    auto spriteBox = tile->addComponent<SpriteComponent>();
                    auto sprite = MAMGame::instance->getSprite(tileId);
                    spriteBox->setSprite(sprite);
                    auto phys = tile->addComponent<PhysicsComponent>();
                    b2Vec2 vertices[3];
                    glm::vec2 size = getTileSize() / MAMGame::instance->physicsScale;
                    vertices[0].Set(-size.x, -size.y);
                    vertices[1].Set(size.x, size.y);
                    vertices[2].Set(size.x, -size.y);
                    phys->initTriangle(b2_staticBody, size, tile->getPosition(), vertices, 1, 1);
                    break;
                }
                case 9: { // make bottom left triangle
                    glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight);
                    auto tile = MAMGame::instance->createGameObject(position, GOType::slope);
                    auto spriteBox = tile->addComponent<SpriteComponent>();
                    auto sprite = MAMGame::instance->getSprite(tileId);
                    spriteBox->setSprite(sprite);
                    auto phys = tile->addComponent<PhysicsComponent>();
                    b2Vec2 vertices[3];
                    glm::vec2 size = getTileSize() / MAMGame::instance->physicsScale;
                    vertices[0].Set(-size.x, size.y);
                    vertices[1].Set(-size.x, -size.y);
                    vertices[2].Set(size.x, -size.y);
                    phys->initTriangle(b2_staticBody, size, tile->getPosition(), vertices, 1, 1);
                    break;
                }
                case 23: {
                    glm::vec2 position = glm::vec2(xPos * tileWidth, yPos * tileHeight + 15);
                    auto tile = MAMGame::instance->createGameObject(position, GOType::target);
                    auto spriteBox = tile->addComponent<SpriteComponent>();
                    auto sprite = MAMGame::instance->getSprite(tileId);
                    spriteBox->setSprite(sprite);
                    auto phys = tile->addComponent<PhysicsComponent>();
                    phys->initTarget(b2_staticBody, tile->getPosition());
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