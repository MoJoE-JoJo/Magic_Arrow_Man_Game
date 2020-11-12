#pragma once
#include "GameObject.hpp"
#include "PlayerController.hpp"

class PlayerObject : public GameObject {
public:
    explicit PlayerObject(glm::vec2 pos): GameObject(pos) {}
    void update(float deltaTime) override;
private:
    bool movingLeft = false;
    bool movingRight = false;
    friend class PlayerController;
};
