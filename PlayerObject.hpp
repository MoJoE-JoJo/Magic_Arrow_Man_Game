#pragma once
#include "GameObject.hpp"
#include "BowObject.hpp"
#include "PlayerController.hpp"

class BowObject;

class PlayerObject : public GameObject {
public:
    PlayerObject(glm::vec2 pos, sre::Sprite walk1, sre::Sprite standing, sre::Sprite walk2);
    void update(float deltaTime) override;
    void jump();
    void incrGroundCounter();
    void decrGroundCounter(); 
    void incrSlopeCounter();
    void decrSlopeCounter();
    bool isGrounded();
    void setOnLeftSlope(bool newOnSlope);
    void setOnRightSlope(bool newOnSlope);
private:
    bool movingLeft = false;
    bool movingRight = false;
    bool onLeftSlope = false;
    bool onRightSlope = false;
    sre::Sprite walk1;
    sre::Sprite walk2;
    sre::Sprite standing;
    int groundCounter = 0;
    int slopeCounter = 0;

    float walkingSpriteIndicator = 0;
    int whichWalkIndicator = 0;
    bool flipIndicator = false;

    void updateSprite(float deltaTime);
    friend class PlayerController;
};
