#pragma once
#include "sre/SpriteAtlas.hpp"

#include "PlayerObject.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerPhysics.hpp"
#include "SpriteComponent.hpp"
#include "MAMGame.hpp"

PlayerObject::PlayerObject(glm::vec2 pos, sre::Sprite walk1, sre::Sprite standing, sre::Sprite walk2) : GameObject(pos, GOType::player) {
    auto pSpriteBox = addComponent<SpriteComponent>();
    pSpriteBox->setSprite(standing);
    auto phys = addComponent<PlayerPhysics>();
    phys->initBox(b2_dynamicBody, glm::vec2((standing.getSpriteSize().x - 15) / 2, (standing.getSpriteSize().y - 25) / 2), getPosition(), 0.2f);
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