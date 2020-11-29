#pragma once
#include "GameObject.hpp"
#include "ArrowPhysics.hpp"
#include "BowObject.hpp"
#include "MAMGame.hpp"

ArrowPhysics::ArrowPhysics(GameObject* gameObject) : PhysicsComponent(gameObject) {
	b2Vec2 vertices[4];
	vertices[0].Set(-30 / MAMGame::instance->physicsScale, 10 / MAMGame::instance->physicsScale);
	vertices[1].Set(-30 / MAMGame::instance->physicsScale, -10 / MAMGame::instance->physicsScale);
	vertices[2].Set(30 / MAMGame::instance->physicsScale, -10 / MAMGame::instance->physicsScale);
	vertices[3].Set(30 / MAMGame::instance->physicsScale, 10 / MAMGame::instance->physicsScale);

	initPolygon(b2_dynamicBody, gameObject->getPosition() * 100000.0f, 0.2, vertices, 4, 0.1);
	body->SetSleepingAllowed(false);
	setSensor(true);
}

void ArrowPhysics::onCollisionStart(PhysicsComponent* comp) {
	collisionCounter++;
	if (comp->getGameObject()->goType == GOType::bow) {
		auto bow = dynamic_cast<BowObject*>(comp->getGameObject());
		bow->arrowReturned();
		returnToBow();
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

void ArrowPhysics::shoot(float bowForce) {
	setSensor(false);
	setLinearVelocity(glm::vec2(0, 0));

	float angle = std::fmod(gameObject->getRotation(), 360);
	angle = angle * (3.14 / 180);
	float force = bowForce * getMass();
	addForce(glm::vec2(cos(angle), sin(angle)) * force);
}

void ArrowPhysics::returnToBow() {
	setSensor(true);
	isReturned = true;
	body->SetAngularVelocity(0);
}