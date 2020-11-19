#pragma once
#include "PhysicsComponent.hpp"
#include "PlayerObject.hpp"

class PlayerPhysics : public PhysicsComponent {
public:
    explicit PlayerPhysics(GameObject* gameObject) : PhysicsComponent(gameObject) {}
    void onCollisionStart(PhysicsComponent* comp) override;
    void onCollisionEnd(PhysicsComponent* comp) override;
private:
    
};
