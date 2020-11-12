#pragma once
#include "PlayerObject.hpp"
#include "PhysicsComponent.hpp"
#include "MAMGame.hpp"

void PlayerObject::update(float deltaTime) {
	GameObject::update(deltaTime);
    if (movingRight && isGrounded) {
        getComponent<PhysicsComponent>()->addForce(glm::vec2(10, 0));
    }
    if (movingLeft && isGrounded) {
        getComponent<PhysicsComponent>()->addForce(glm::vec2(-10, 0));
    }
}

void PlayerObject::jump() {
    getComponent<PhysicsComponent>()->addForce(glm::vec2(0, 200));
}

void PlayerObject::setGrounded(bool grounded) {
    isGrounded = grounded;
}