#pragma once
#include "PlayerPhysics.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"

void PlayerPhysics::onCollisionStart(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->setGrounded(true);
	}
}

void PlayerPhysics::onCollisionEnd(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->setGrounded(false);
	}
}