#pragma once
#include "sre/SDLRenderer.hpp"

#include "PlayerObject.hpp"
#include "BowObject.hpp"

class PlayerObject;
class BowObject;

class PlayerController {
public:
    PlayerController(std::shared_ptr<PlayerObject> player);
    ~PlayerController();
    void onKey(SDL_Event& event);
    void mouseEvent(SDL_Event& event, glm::vec2 pos);
    void setBow(std::shared_ptr<BowObject> bow);
    void update();
private:
    std::shared_ptr<PlayerObject> player;
    std::shared_ptr<BowObject> bow;
    bool bowIsSet = false;
    friend class PlayerObject;
    friend class BowObject;
    friend class MAMGame;
};