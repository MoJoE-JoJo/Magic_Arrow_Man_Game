#pragma once
#include "sre/SDLRenderer.hpp"

#include "PlayerObject.hpp"

class PlayerObject;

class PlayerController {
public:
    PlayerController(std::shared_ptr<PlayerObject> player);
    ~PlayerController();
    void onKey(SDL_Event& event);
    void mouseEvent(SDL_Event& event, glm::vec2 pos);
    void reset();
    glm::vec2 getPlayerPosition();
    std::shared_ptr<PlayerObject> getPlayer();
private:
    std::shared_ptr<PlayerObject> player;
};