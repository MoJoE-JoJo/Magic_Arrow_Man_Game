#pragma once
#include "sre/SpriteAtlas.hpp"

#include "PlayerObject.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerPhysics.hpp"
#include "SpriteComponent.hpp"
#include "MAMGame.hpp"

PlayerObject::PlayerObject(glm::vec2 pos, sre::Sprite walk1, sre::Sprite standing, sre::Sprite walk2) : GameObject(pos, GOType::player) {
    auto pSpriteBox = addComponent<SpriteComponent>();
    this->walk1 = walk1;
    this->walk2 = walk2;
    this->standing = standing;
    pSpriteBox->setSprite(standing);
    auto phys = addComponent<PlayerPhysics>();
    b2Vec2 vertices[8];
    vertices[0].Set(-25.0f / MAMGame::instance->physicsScale,   20.0f / MAMGame::instance->physicsScale);
    vertices[1].Set(-20.0f / MAMGame::instance->physicsScale,   -20.0f / MAMGame::instance->physicsScale);
    vertices[2].Set(-10.0f / MAMGame::instance->physicsScale,   -30.0f / MAMGame::instance->physicsScale);
    vertices[3].Set(10.0f / MAMGame::instance->physicsScale,    -30.0f / MAMGame::instance->physicsScale);
    vertices[4].Set(25.0f / MAMGame::instance->physicsScale,    20.0f / MAMGame::instance->physicsScale);
    vertices[5].Set(20.0f / MAMGame::instance->physicsScale,    -20.0f / MAMGame::instance->physicsScale);
    vertices[6].Set(-10.0f / MAMGame::instance->physicsScale,   30.0f / MAMGame::instance->physicsScale);
    vertices[7].Set(10.0f / MAMGame::instance->physicsScale,    30.0f / MAMGame::instance->physicsScale);

    phys->initPolygon(b2_dynamicBody, getPosition(), 0.2f, vertices, 8, 0.2);
}

void PlayerObject::update(float deltaTime) {
	GameObject::update(deltaTime);
    auto phys = getComponent<PhysicsComponent>();
    if (movingRight && isGrounded()) {
        phys->addForce(glm::vec2(1000 * phys->getMass(), 0));
    }
    if (movingLeft && isGrounded()) {
        phys->addForce(glm::vec2(-1000 * phys->getMass(), 0));
    }
    updateSprite(deltaTime);
}

void PlayerObject::jump() {
    auto phys = getComponent<PhysicsComponent>();
    phys->addForce(glm::vec2(0, 20000 * phys->getMass()));
}

void PlayerObject::incrCollisionCounter() {
    collisionCounter++;
}

void PlayerObject::decrCollisionCounter() {
    collisionCounter--;
}

bool PlayerObject::isGrounded() {
    return collisionCounter != 0;
}

void PlayerObject::updateSprite(float deltaTime) {
    auto phys = getComponent<PhysicsComponent>();
    auto velocity = phys->getLinearVelocity();

    auto newSprite = standing;
    if (velocity.x == 0 && velocity.y == 0) {
        newSprite = standing;
        whichWalkIndicator = 2;
        walkingSpriteIndicator = 0;
    } else if (!isGrounded()) {
        newSprite = walk1;
        whichWalkIndicator = 2;
        walkingSpriteIndicator = 0;
    } else {
        if (velocity.x > 0) walkingSpriteIndicator += deltaTime * velocity.x;
        else walkingSpriteIndicator += deltaTime * velocity.x * -1;
        
        if (walkingSpriteIndicator > 1.8) {
            whichWalkIndicator = 1;
            walkingSpriteIndicator = 0;
        } else if (walkingSpriteIndicator > 1.2) {
            whichWalkIndicator = 0;
        } else if (walkingSpriteIndicator > 0.9) {
            whichWalkIndicator = 1;
        } else if (walkingSpriteIndicator > 0.3) {
            whichWalkIndicator = 2;
        }

        if (whichWalkIndicator == 0) newSprite = walk1;
        else if (whichWalkIndicator == 1) newSprite = standing;
        else if (whichWalkIndicator == 2) newSprite = walk2;
    }

    if (movingLeft) flipIndicator = true;
    else if (movingRight) flipIndicator = false;
    
    newSprite.setFlip({ flipIndicator, newSprite.getFlip().y });
    getComponent<SpriteComponent>()->setSprite(newSprite);
}