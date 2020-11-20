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
private:
    std::shared_ptr<PlayerObject> player;
    
    friend class PlayerObject;
    friend class MAMGame;
};