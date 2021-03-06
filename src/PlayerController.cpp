#pragma once
#include "PlayerController.hpp"
#include "PlayerObject.hpp"

PlayerController::PlayerController(std::shared_ptr<PlayerObject> player) {
    this->player = player;
}

PlayerController::~PlayerController() {
    player = nullptr;
}

void PlayerController::onKey(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_d) {
            player->setMovingRight(true);
        } else if (event.key.keysym.sym == SDLK_a) {
            player->setMovingLeft(true);
        } else if (event.key.keysym.sym == SDLK_SPACE && player->isGrounded()) {
            player->jump();
        }
    } else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_d) {
            player->setMovingRight(false);
        } else if (event.key.keysym.sym == SDLK_a) {
            player->setMovingLeft(false);
        }
    }
}

void PlayerController::mouseEvent(SDL_Event& event, glm::vec2 pos) {
    player->useBow(event, pos);
}

void PlayerController::reset() {
    player->reset();
}

glm::vec2 PlayerController::getPlayerPosition() {
    return player->getPosition();
}

std::shared_ptr<PlayerObject> PlayerController::getPlayer() {
    return player;
}