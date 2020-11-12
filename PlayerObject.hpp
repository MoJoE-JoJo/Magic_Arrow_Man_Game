#pragma once
#include "GameObject.hpp"
#include "PlayerController.hpp"

class PlayerObject : public GameObject {
public:
    PlayerObject(glm::vec2 pos);
    void update(float deltaTime) override;
    void jump();
    void setGrounded(bool grounded);
private:
    bool movingLeft = false;
    bool movingRight = false;
    bool isGrounded = true;
    friend class PlayerController;
};
