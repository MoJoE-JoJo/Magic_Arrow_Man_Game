#pragma once
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerObject.hpp"

PlayerController::PlayerController(std::shared_ptr<PlayerObject> player) {
    this->player = player;
}

PlayerController::~PlayerController() {
    player = nullptr;
    bow = nullptr;
}

void PlayerController::onKey(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_d) {
            player->movingRight = true;
        } else if (event.key.keysym.sym == SDLK_a) {
            player->movingLeft = true;
        } else if (event.key.keysym.sym == SDLK_SPACE && player->isGrounded()) {
            player->jump();
        }
    } else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_d) {
            player->movingRight = false;
        } else if (event.key.keysym.sym == SDLK_a) {
            player->movingLeft = false;
        }
    }
}

void PlayerController::mouseEvent(SDL_Event& event, glm::vec2 pos) {
    if (bowIsSet) {
        bow->updateAngle(pos);
    }
}

void PlayerController::setBow(std::shared_ptr<BowObject> bow) {
    this->bow = bow;
}

void PlayerController::update() {
    if (bowIsSet) {
        auto offset = player->flipIndicator ? glm::vec2(-15, -13) : glm::vec2(15, -13);
        auto position = player->getPosition() + offset;
        bow->updatePos(position);
    }
}