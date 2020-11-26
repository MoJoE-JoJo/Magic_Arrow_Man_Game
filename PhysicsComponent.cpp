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
    center = center / MAMGame::instance->physicsScale;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    body->SetFixedRotation(true);
    polygon = new b2PolygonShape();
    size = size / MAMGame::instance->physicsScale;
    polygon->SetAsBox(size.x, size.y, { 0, 0 }, 0);
    b2FixtureDef fxD;
    fxD.shape = polygon;
    fxD.friction = 1;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);

    MAMGame::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initTriangle(b2BodyType type, glm::vec2 center, b2Vec2 vertices[], float density) {
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
    polygon->Set(vertices, 3);
    b2FixtureDef fxD;
    fxD.shape = polygon;
    fxD.friction = 1;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);

    MAMGame::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initCircle(b2BodyType type, float radius, glm::vec2 center, float density) {
    assert(body == nullptr);
    // do init
    shapeType = b2Shape::Type::e_circle;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    center = center / MAMGame::instance->physicsScale;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    body->SetFixedRotation(true);
    circle = new b2CircleShape();
    circle->m_radius = radius / MAMGame::instance->physicsScale;
    b2FixtureDef fxD;
    fxD.shape = circle;
    fxD.friction = 1;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);

    MAMGame::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initPolygon(b2BodyType type, glm::vec2 center, float density, b2Vec2* vertices, int32 vertexCount, float friction) {
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

    //b2PolygonShape polygon;

    polygon->Set(vertices, vertexCount);
    b2FixtureDef fxD;
    fxD.shape = polygon;
    fxD.density = density;
    fxD.friction = friction;
    fixture = body->CreateFixture(&fxD);

    MAMGame::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initTarget(b2BodyType type, glm::vec2 center) {
    assert(body == nullptr);
    // do init
    shapeType = b2Shape::Type::e_circle;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    center = (center + glm::vec2(0, 7)) / MAMGame::instance->physicsScale;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    body->SetFixedRotation(true);
    circle = new b2CircleShape();
    circle->m_radius = 40.0f / MAMGame::instance->physicsScale;
    b2FixtureDef fxD;
    fxD.shape = circle;
    fxD.density = 1;
    fixture = body->CreateFixture(&fxD);

    MAMGame::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initArrow(b2BodyType type, glm::vec2 center) {
    assert(body == nullptr);
    // do init
    shapeType = b2Shape::Type::e_polygon;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    center = center / MAMGame::instance->physicsScale;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    body->SetFixedRotation(false);
    body->SetSleepingAllowed(false);
    polygon = new b2PolygonShape();

    b2Vec2 vertices[4];
    vertices[0].Set(-30 / MAMGame::instance->physicsScale, 10 / MAMGame::instance->physicsScale);
    vertices[1].Set(-30 / MAMGame::instance->physicsScale, -10 / MAMGame::instance->physicsScale);
    vertices[2].Set(30 / MAMGame::instance->physicsScale, -10 / MAMGame::instance->physicsScale);
    vertices[3].Set(30 / MAMGame::instance->physicsScale, 10 / MAMGame::instance->physicsScale);

    polygon->Set(vertices, 4);
    b2FixtureDef fxD;
    fxD.shape = polygon;
    fxD.density = 0.2;
    fxD.friction = 0.1;
    fixture = body->CreateFixture(&fxD);
    fixture->SetSensor(true);

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

void PhysicsComponent::setPosition(b2Vec2 position) {
    body->SetTransform(position, body->GetTransform().q.GetAngle());
}

void PhysicsComponent::onCollisionStart(PhysicsComponent* comp) { }

void PhysicsComponent::onCollisionEnd(PhysicsComponent* comp) { }

float PhysicsComponent::getMass() {
    return body->GetMass();
}

void PhysicsComponent::setPosition(glm::vec2 pos) {
    body->SetTransform(b2Vec2(pos.x, pos.y), body->GetAngle());
}

void PhysicsComponent::setRotation(float angle) {
    body->SetTransform(body->GetPosition(), angle);
}