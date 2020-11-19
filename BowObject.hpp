#pragma once
#include "GameObject.hpp"
#include "PlayerController.hpp"

class BowObject : public GameObject {
public:
    BowObject(glm::vec2 pos, sre::Sprite bowSprite);
    void update(float deltaTime) override;
    void updatePos(glm::vec2 pos);
private:
    friend class PlayerController;
};
