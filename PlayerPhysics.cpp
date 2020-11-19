#pragma once
#include "PlayerPhysics.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"
#include "BowObject.hpp"
#include "MAMGame.hpp"

void PlayerPhysics::onCollisionStart(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground || comp->getGameObject()->goType == GOType::slope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->incrCollisionCounter();
	} else if (comp->getGameObject()->goType == GOType::target) {
		MAMGame::instance->setGameState(GameState::Won);
	} else if (comp->getGameObject()->goType == GOType::bow) {
		MAMGame::instance->setBow();
	}
}

void PlayerPhysics::onCollisionEnd(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground || comp->getGameObject()->goType == GOType::slope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->decrCollisionCounter();
	}
}