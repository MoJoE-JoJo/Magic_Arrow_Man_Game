#pragma once
#include "PlayerPhysics.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"
#include "BowObject.hpp"
#include "MAMGame.hpp"

void PlayerPhysics::onCollisionStart(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->incrGroundCounter();
	} else if (comp->getGameObject()->goType == GOType::leftSlope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->incrSlopeCounter();
		player->setOnLeftSlope(true);
	} else if (comp->getGameObject()->goType == GOType::rightSlope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->incrSlopeCounter();
		player->setOnRightSlope(true);
	} else if (comp->getGameObject()->goType == GOType::target) {
		MAMGame::instance->setGameState(GameState::Won);
	} else if (comp->getGameObject()->goType == GOType::bow) {
		MAMGame::instance->setBow();
	}
}

void PlayerPhysics::onCollisionEnd(PhysicsComponent* comp) {
	if (comp->getGameObject()->goType == GOType::ground) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->decrGroundCounter();
	} else if (comp->getGameObject()->goType == GOType::leftSlope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->decrSlopeCounter();
		player->setOnLeftSlope(false);
	} else if (comp->getGameObject()->goType == GOType::rightSlope) {
		auto player = dynamic_cast<PlayerObject*>(gameObject);
		player->decrSlopeCounter();
		player->setOnRightSlope(false);
	}
}