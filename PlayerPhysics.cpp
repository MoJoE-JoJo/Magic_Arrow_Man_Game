#pragma once
#include "PlayerPhysics.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"

void PlayerPhysics::onCollisionStart(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground || comp->getGameObject()->goType == GOType::slope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->incrCollisionCounter();
	}
}

void PlayerPhysics::onCollisionEnd(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground || comp->getGameObject()->goType == GOType::slope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->decrCollisionCounter();
	}
}