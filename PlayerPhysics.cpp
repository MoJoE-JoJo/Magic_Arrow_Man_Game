#pragma once
#include "PlayerPhysics.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"
#include "BowObject.hpp"
#include "MAMGame.hpp"

void PlayerPhysics::onCollisionStart(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->incrCollisionCounter();
	} else if (comp->getGameObject()->goType == GOType::leftSlope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->incrCollisionCounter();
		player->setOnLeftSlope(true);
	} else if (comp->getGameObject()->goType == GOType::rightSlope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->incrCollisionCounter();
		player->setOnRightSlope(true);
	} else if (comp->getGameObject()->goType == GOType::bow) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->pickUpBow();
	}
}

void PlayerPhysics::onCollisionEnd(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->decrCollisionCounter();
	} else if (comp->getGameObject()->goType == GOType::leftSlope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->decrCollisionCounter();
		player->setOnLeftSlope(false);
	} else if (comp->getGameObject()->goType == GOType::rightSlope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->decrCollisionCounter();
		player->setOnRightSlope(false);
	}
}