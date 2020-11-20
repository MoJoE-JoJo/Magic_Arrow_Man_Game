#pragma once
#include "sre/SpriteAtlas.hpp"

#include "BowObject.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "MAMGame.hpp"

BowObject::BowObject(glm::vec2 pos, sre::Sprite bowSprite) : GameObject(pos, GOType::bow) {
    auto bowSpriteBox = addComponent<SpriteComponent>();
    bowSpriteBox->setSprite(bowSprite);
    auto phys = addComponent<PhysicsComponent>();
    b2Vec2 vertices[4];
    vertices[0].Set(-10 / MAMGame::instance->physicsScale, 31 / MAMGame::instance->physicsScale);
    vertices[1].Set(-10 / MAMGame::instance->physicsScale, -31 / MAMGame::instance->physicsScale);
    vertices[2].Set(11 / MAMGame::instance->physicsScale, -31 / MAMGame::instance->physicsScale);
    vertices[3].Set(11 / MAMGame::instance->physicsScale, 30 / MAMGame::instance->physicsScale);

    phys->initPolygon(b2_dynamicBody, getPosition(), 1, vertices, 4, 1);
    phys->setSensor(true);
}

void BowObject::update(float deltaTime) {
    GameObject::update(deltaTime);
}

void BowObject::updatePos(glm::vec2 pos) {
    getComponent<PhysicsComponent>()->setPosition(pos / MAMGame::instance->physicsScale);
    //setPosition(pos);
}

void BowObject::updateAngle(glm::vec2 mousePos) {
    b2Vec2 toTarget = b2Vec2(mousePos.x, mousePos.y) - b2Vec2(getPosition().x, getPosition().y);
    float angle = glm::degrees(atan2f(-toTarget.x, toTarget.y)) + 90;
    getComponent<PhysicsComponent>()->setRotation(glm::radians(angle));
    //setRotation(angle);
}