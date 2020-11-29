#pragma once
#include "../../../GameObject.hpp"
#include "ArrowPhysics.hpp"
#include "../../../BowObject.hpp"
#include "../../../MAMGame.hpp"

void ArrowPhysics::onCollisionStart(PhysicsComponent* comp) {
	collisionCounter++;
	if (comp->getGameObject()->goType == GOType::bow) {
		auto bow = dynamic_cast<BowObject*>(comp->getGameObject());
		bow->arrowReturned();
		setSensor(true);
		isReturned = true;
		body->SetAngularVelocity(0);
	} else if (comp->getGameObject()->goType == GOType::target) {
		if (!isReturned) {
			MAMGame::instance->audioSystem.playSound(SoundType::ArrowHitting, 100);
			MAMGame::instance->levelWon();
		}
	}
}

void ArrowPhysics::onCollisionEnd(PhysicsComponent* comp) {
	collisionCounter--;
	if (comp->getGameObject()->goType == GOType::bow) {
		isReturned = false;
	}
}

void ArrowPhysics::update(float deltaTime) {
	PhysicsComponent::update(deltaTime);
	if (!isReturned) {
		auto velocity = body->GetLinearVelocity();
		if (velocity.x > 0.1 && velocity.y > 0.1) {
			float angle = glm::radians(glm::degrees(atan2((double)velocity.y, (double)velocity.x)) + 90);
			body->SetTransform(body->GetPosition(), angle - (3.14159265) / 2.0f);
		}
	}
}

void ArrowPhysics::setRotation(float angle) {
	if (collisionCounter == 0 || isReturned) {
		body->SetTransform(body->GetPosition(), angle);
	}
}