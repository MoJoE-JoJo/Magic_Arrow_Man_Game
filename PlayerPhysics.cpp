#pragma once
#include "PlayerPhysics.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"

void PlayerPhysics::onCollisionStart(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground) {
		std::cout << comp->getGameObject()->getPosition().x << " " << comp->getGameObject()->getPosition().y << " ";
		std::cout << gameObject->getPosition().x << " " << gameObject->getPosition().y << std::endl;
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->incrCollisionCounter();
	}
}

void PlayerPhysics::onCollisionEnd(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->decrCollisionCounter();
	}
}