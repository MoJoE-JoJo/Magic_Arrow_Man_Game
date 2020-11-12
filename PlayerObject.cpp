#pragma once
#include "PlayerObject.hpp"
#include "PhysicsComponent.hpp"
#include "MAMGame.hpp"

void PlayerObject::update(float deltaTime) {
	GameObject::update(deltaTime);
    if (movingRight) {
        getComponent<PhysicsComponent>()->addForce(glm::vec2(10, 0));
    }
    if (movingLeft) {
        getComponent<PhysicsComponent>()->addForce(glm::vec2(-10, 0));
    }
}