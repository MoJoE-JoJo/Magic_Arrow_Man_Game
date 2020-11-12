#pragma once
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerObject.hpp"

PlayerController::PlayerController(std::shared_ptr<PlayerObject> player) {
    this->player = player;
}

PlayerController::~PlayerController() {
    player = nullptr;
}

void PlayerController::onKey(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RIGHT) {
            player->movingRight = true;
        } else if (event.key.keysym.sym == SDLK_LEFT) {
            player->movingLeft = true;
        }
    } else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_RIGHT) {
            player->movingRight = false;
            auto yy = player->getComponent<PhysicsComponent>()->getLinearVelocity().y;
            player->getComponent<PhysicsComponent>()->setLinearVelocity(glm::vec2(0, yy));
        } else if (event.key.keysym.sym == SDLK_LEFT) {
            player->movingLeft = false;
            auto yy = player->getComponent<PhysicsComponent>()->getLinearVelocity().y;
            player->getComponent<PhysicsComponent>()->setLinearVelocity(glm::vec2(0, yy));
        }
    }
}