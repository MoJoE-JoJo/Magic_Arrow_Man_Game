#pragma once
#include "PhysicsComponent.hpp"

class ArrowPhysics : public PhysicsComponent {
public:
    ArrowPhysics(GameObject* gameObject);
    void onCollisionStart(PhysicsComponent* comp) override;
    void onCollisionEnd(PhysicsComponent* comp) override;
    void update(float deltaTime) override;
    void setRotation(float angle) override;
    void shoot(float bowForce);
private:
    void returnToBow();

    bool isReturned = true;
    int collisionCounter = 0;
};
