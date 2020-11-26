#pragma once
#include "GameObject.hpp"
#include "BowObject.hpp"
#include "PlayerController.hpp"

class BowObject;

class PlayerObject : public GameObject {
public:
    PlayerObject(glm::vec2 pos, sre::Sprite walk1, sre::Sprite standing, sre::Sprite walk2);
    ~PlayerObject();
    void update(float deltaTime) override;
    void jump();
    void incrGroundCounter();
    void decrGroundCounter(); 
    void incrSlopeCounter();
    void decrSlopeCounter();
    bool isGrounded();
    void setOnLeftSlope(bool newOnSlope);
    void setOnRightSlope(bool newOnSlope);
    void setBow(std::shared_ptr<BowObject> bow, bool samePosAsPlayer);
    void pickUpBow();
    void useBow(SDL_Event& event, glm::vec2 pos);
    void reset();
private:
    bool decelerate = false;
    bool movingLeft = false;
    bool movingRight = false;
    bool callingArrow = false;
    bool hasCalledArrowOnceInAir = false;
    bool stoppedCallingArrow = false;
    bool onLeftSlope = false;
    bool onRightSlope = false;
    sre::Sprite walk1;
    sre::Sprite walk2;
    sre::Sprite standing;
    int groundCounter = 0;
    int slopeCounter = 0;
    int speed = 1000;
    float playerDeceleration = 5.0f;
    float maxVelocity = 10.0f;

    glm::vec2 originalPosition;
    bool samePosAsPlayer = false;

    float walkingSpriteIndicator = 0;
    int whichWalkIndicator = 0;
    bool flipIndicator = false;
    std::shared_ptr<BowObject> bow;
    bool bowIsSet = false;

    void updateSprite(float deltaTime);
    void stopAfterFlying();
    friend class PlayerController;
};
