#pragma once
#include "PhysicsComponent.hpp"

class ArrowPhysics : public PhysicsComponent {
public:
    explicit ArrowPhysics(GameObject* gameObject) : PhysicsComponent(gameObject) {}
    void onCollisionStart(PhysicsComponent* comp) override;
    void onCollisionEnd(PhysicsComponent* comp) override;
    void update(float deltaTime) override;
    void setRotation(float angle) override;
private:
    bool isReturned = true;
    int collisionCounter = 0;
};
