#pragma once
#include "Box2D/Common/b2Math.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Dynamics/b2Body.h"
#include <glm/glm.hpp>
#include <vector>

#include "Component.hpp"

class PhysicsComponent : public Component {
public:
    explicit PhysicsComponent(GameObject* gameObject);
    virtual ~PhysicsComponent();
    void initBox(b2BodyType type, glm::vec2 size, glm::vec2 center, float density);
    void addForce(glm::vec2 force);
    void addImpulse(glm::vec2 impulse);
    void setLinearVelocity(glm::vec2 velocity);
    glm::vec2 getLinearVelocity();
    bool isSensor();
    void setSensor(bool enabled);
    void update(float deltaTime) override;
private:
    b2PolygonShape* polygon = nullptr;
    b2CircleShape* circle = nullptr;
    b2Body* body = nullptr;
    b2Shape::Type shapeType;
    b2Fixture* fixture = nullptr;
    b2BodyType rbType;
    std::vector<PhysicsComponent*> collidingBodies;
    b2World* world = nullptr;
    friend class MAMGame;
};

