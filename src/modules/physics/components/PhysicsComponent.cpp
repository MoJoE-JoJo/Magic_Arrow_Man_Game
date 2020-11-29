#pragma once
#include <Box2D/Box2D.h>
#include <iostream>
#include <math.h>

#include "PhysicsComponent.hpp"
#include "../../../MAMGame.hpp"

PhysicsComponent::PhysicsComponent(GameObject* gameObject) : Component(gameObject) {
    world = MAMGame::instance->world;
}

PhysicsComponent::~PhysicsComponent() {
    MAMGame::instance->deregisterPhysicsComponent(this);

    delete polygon;
    if (body != nullptr && fixture != nullptr) {
        body->DestroyFixture(fixture);
        fixture = nullptr;
    }
    if (world != nullptr && body != nullptr) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

void PhysicsComponent::init(b2BodyType type, glm::vec2 center) {
    assert(body == nullptr);
    // do init
    shapeType = b2Shape::Type::e_polygon;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    center = center / MAMGame::instance->physicsScale;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    body->SetFixedRotation(true);
    polygon = new b2PolygonShape();
}

void PhysicsComponent::initBox(b2BodyType type, glm::vec2 size, glm::vec2 center, float density) {
    init(type, center);
    size = size / MAMGame::instance->physicsScale;
    polygon->SetAsBox(size.x, size.y, { 0, 0 }, 0);
    b2FixtureDef fxD;
    fxD.shape = polygon;
    fxD.friction = 1;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);

    MAMGame::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initPolygon(b2BodyType type, glm::vec2 center, float density, b2Vec2* vertices, int32 vertexCount, float friction) {
    init(type, center);
    polygon->Set(vertices, vertexCount);
    b2FixtureDef fxD;
    fxD.shape = polygon;
    fxD.density = density;
    fxD.friction = friction;
    fixture = body->CreateFixture(&fxD);

    MAMGame::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::update(float deltaTime) {
    if (rbType == b2_staticBody) return;
    auto position = body->GetPosition();
    float angle = glm::degrees(body->GetAngle());
    getGameObject()->setPosition(glm::vec2(position.x, position.y) * MAMGame::instance->physicsScale);
    getGameObject()->setRotation(angle);
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

float PhysicsComponent::getMass() {
    return body->GetMass();
}

bool PhysicsComponent::isSensor() {
    return fixture->IsSensor();
}

void PhysicsComponent::setSensor(bool enabled) {
    fixture->SetSensor(enabled);
}

void PhysicsComponent::setPosition(glm::vec2 pos) {
    body->SetTransform(b2Vec2(pos.x, pos.y), body->GetAngle());
}

void PhysicsComponent::setRotation(float angle) {
    body->SetTransform(body->GetPosition(), angle);
}

void PhysicsComponent::onCollisionStart(PhysicsComponent* comp) {}

void PhysicsComponent::onCollisionEnd(PhysicsComponent* comp) {}