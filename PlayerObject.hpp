#pragma once
#include "GameObject.hpp"
#include "PlayerController.hpp"

class PlayerObject : public GameObject {
public:
    PlayerObject(glm::vec2 pos, sre::Sprite walk1, sre::Sprite standing, sre::Sprite walk2);
    void update(float deltaTime) override;
    void jump();
    void incrCollisionCounter();
    void decrCollisionCounter();
    bool isGrounded();
private:
    bool movingLeft = false;
    bool movingRight = false;
    sre::Sprite walk1;
    sre::Sprite walk2;
    sre::Sprite standing;
    int collisionCounter = 0;
    float walkingSpriteIndicator = 0;
    int whichWalkIndicator = 0;

    void updateSprite(float deltaTime);
    friend class PlayerController;
};
