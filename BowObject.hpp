#pragma once
#include "sre/SDLRenderer.hpp"

#include "GameObject.hpp"

class BowObject : public GameObject {
public:
    BowObject(glm::vec2 pos, sre::Sprite bowSprite, std::shared_ptr<GameObject> arrow);
    ~BowObject();
    void updatePos(glm::vec2 pos);
    void updateAngle(glm::vec2 mousePos);
    void shootArrow();
    void arrowReturned();
private:
    std::shared_ptr<GameObject> arrow;
    bool hasArrow = true;
};
