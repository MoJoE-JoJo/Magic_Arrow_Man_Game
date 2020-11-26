#pragma once
#include "sre/SpriteAtlas.hpp"

#include "BowObject.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "MAMGame.hpp"

BowObject::BowObject(glm::vec2 pos, sre::Sprite bowSprite, std::shared_ptr<GameObject> arrow) : GameObject(pos, GOType::bow) {
    auto bowSpriteBox = addComponent<SpriteComponent>();
    bowSpriteBox->setSprite(bowSprite);
    auto phys = addComponent<PhysicsComponent>();
    b2Vec2 vertices[4];
    vertices[0].Set(-10 / MAMGame::instance->physicsScale, 31 / MAMGame::instance->physicsScale);
    vertices[1].Set(-10 / MAMGame::instance->physicsScale, -31 / MAMGame::instance->physicsScale);
    vertices[2].Set(11 / MAMGame::instance->physicsScale, -31 / MAMGame::instance->physicsScale);
    vertices[3].Set(11 / MAMGame::instance->physicsScale, 30 / MAMGame::instance->physicsScale);

    phys->initPolygon(b2_kinematicBody, getPosition(), 1, vertices, 4, 1);
    phys->setSensor(true);

    this->arrow = arrow;
    originalPosition = pos / MAMGame::instance->physicsScale;
}

BowObject::~BowObject() {
    arrow = nullptr;
}

void BowObject::updatePos(glm::vec2 pos) {
    getComponent<PhysicsComponent>()->setPosition(pos / MAMGame::instance->physicsScale);
    if (hasArrow) {
        arrow->getComponent<PhysicsComponent>()->setPosition(pos / MAMGame::instance->physicsScale);
    }
}

void BowObject::updateAngle(glm::vec2 mousePos) {
    b2Vec2 toTarget = b2Vec2(mousePos.x, mousePos.y) - b2Vec2(getPosition().x, getPosition().y);
    float angle = glm::degrees(atan2f(-toTarget.x, toTarget.y)) + 90;
    getComponent<PhysicsComponent>()->setRotation(glm::radians(angle));
    if (hasArrow) {
        arrow->getComponent<PhysicsComponent>()->setRotation(glm::radians(angle));
    }
}

void BowObject::shootArrow() {
    if (hasArrow) {
        hasArrow = false;
        auto phys = arrow->getComponent<PhysicsComponent>();
        phys->setSensor(false);
        phys->setLinearVelocity(glm::vec2(0, 0));

        float angle = std::fmod(arrow->getRotation(), 360);
        angle = angle * (3.14 / 180);
        float force = 50000 * phys->getMass();
        phys->addForce(glm::vec2(cos(angle), sin(angle)) * force);
    }
}

void BowObject::arrowReturned() {
    hasArrow = true;
}

bool BowObject::isHoldingArrow() {
    return hasArrow;
}

void BowObject::stopArrow() {
    if (!hasArrow) {
        auto phys = arrow->getComponent<PhysicsComponent>();
        auto currentVelocity = phys->getLinearVelocity();
        phys->setLinearVelocity(glm::vec2(0, 0));
        phys->addForce(currentVelocity * 50.0f);
    }
}

void BowObject::callArrow(glm::vec2 playerPos) {
    if (!hasArrow) {
        auto phys = arrow->getComponent<PhysicsComponent>();

        b2Vec2 toTarget = b2Vec2(playerPos.x, playerPos.y) - b2Vec2(arrow->getPosition().x, arrow->getPosition().y);
        float angle = glm::radians(glm::degrees(atan2f(-toTarget.x, toTarget.y)) + 90);
        float force = 500 * phys->getMass();
        phys->setLinearVelocity(glm::vec2(cos(angle), sin(angle)) * force);
        phys->setRotation(angle);
    }
}

glm::vec2 BowObject::getArrowPosition() {
    return arrow->getPosition();
}

void BowObject::reset() {
    hasArrow = true;
    auto phys = getComponent<PhysicsComponent>();
    phys->setPosition(originalPosition);
    phys->setRotation(0);
    auto arrowPhys = arrow->getComponent<PhysicsComponent>();
    arrowPhys->setPosition(originalPosition * 100000.0f);
    arrowPhys->setLinearVelocity(glm::vec2(0, 0));
}