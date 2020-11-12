#pragma once
#include <Box2D/Box2D.h>
#include <iostream>
#include <math.h>

#include "PhysicsComponent.hpp"
#include "MAMGame.hpp"

PhysicsComponent::PhysicsComponent(GameObject* gameObject) : Component(gameObject) {
    world = MAMGame::instance->world;
}

PhysicsComponent::~PhysicsComponent() {
    MAMGame::instance->deregisterPhysicsComponent(this);

    delete polygon;
    delete circle;
    if (body != nullptr && fixture != nullptr) {
        body->DestroyFixture(fixture);
        fixture = nullptr;
    }
    if (world != nullptr && body != nullptr) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

void PhysicsComponent::addImpulse(glm::vec2 impulse) {
    b2Vec2 iForceV{ impulse.x / MAMGame::instance->physicsScale, impulse.y / MAMGame::instance->physicsScale };
    body->ApplyLinearImpulse(iForceV, body->GetWorldCenter(), true);
}

void PhysicsComponent::addForce(glm::vec2 force) {
    b2Vec2 forceV{ force.x / MAMGame::instance->physicsScale, force.y / MAMGame::instance->physicsScale };
    body->ApplyForce(forceV, body->GetWorldCenter(), true);
}

glm::vec2 PhysicsComponent::getLinearVelocity() {
    b2Vec2 v = body->GetLinearVelocity();
    return { v.x,v.y };
}

void PhysicsComponent::setLinearVelocity(glm::vec2 velocity) {
    b2Vec2 v{ velocity.x, velocity.y };
    if (velocity != glm::vec2(0, 0)) {
        body->SetAwake(true);
    }
    body->SetLinearVelocity(v);
}

void PhysicsComponent::initBox(b2BodyType type, glm::vec2 size, glm::vec2 center, float density) {
    assert(body == nullptr);
    // do init
    shapeType = b2Shape::Type::e_polygon;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    polygon = new b2PolygonShape();
    polygon->SetAsBox(size.x, size.y, { 0,0 }, 0);
    b2FixtureDef fxD;
    fxD.shape = polygon;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);

    MAMGame::instance->registerPhysicsComponent(this);
}

bool PhysicsComponent::isSensor() {
    return fixture->IsSensor();
}

void PhysicsComponent::setSensor(bool enabled) {
    fixture->SetSensor(enabled);
}

void PhysicsComponent::update(float deltaTime) {
    if (rbType == b2_staticBody) return;
    auto position = body->GetPosition();
    float angle = glm::degrees(body->GetAngle());
    getGameObject()->setPosition(glm::vec2(position.x, position.y) * MAMGame::instance->physicsScale);
    getGameObject()->setRotation(angle);
}

void PhysicsComponent::onCollisionStart(PhysicsComponent* comp) { }

void PhysicsComponent::onCollisionEnd(PhysicsComponent* comp) { }