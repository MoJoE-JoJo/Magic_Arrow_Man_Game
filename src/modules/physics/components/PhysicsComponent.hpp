#pragma once
#include "Box2D/Common/b2Math.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Dynamics/b2Body.h"
#include <glm/glm.hpp>
#include <vector>

#include "../../../Component.hpp"

class PhysicsComponent : public Component {
public:
    explicit PhysicsComponent(GameObject* gameObject);
    virtual ~PhysicsComponent();
    void initBox(b2BodyType type, glm::vec2 size, glm::vec2 center, float density);
    void initPolygon(b2BodyType type, glm::vec2 center, float density, b2Vec2* vertices, int32 vertexCount, float friction);
    void init(b2BodyType type, glm::vec2 center);
    void update(float deltaTime) override;
    void addForce(glm::vec2 force);
    void addImpulse(glm::vec2 impulse);
    glm::vec2 getLinearVelocity();
    void setLinearVelocity(glm::vec2 velocity);
    bool isSensor();
    void setSensor(bool enabled);
    void setPosition(glm::vec2 pos);
    virtual void setRotation(float angle);
    float getMass();
    virtual void onCollisionStart(PhysicsComponent* comp);
    virtual void onCollisionEnd(PhysicsComponent* comp);
protected:
    b2PolygonShape* polygon = nullptr;
    b2Body* body = nullptr;
    b2Shape::Type shapeType;
    b2Fixture* fixture = nullptr;
    b2BodyType rbType;
    b2World* world = nullptr;
    friend class MAMGame;
};

