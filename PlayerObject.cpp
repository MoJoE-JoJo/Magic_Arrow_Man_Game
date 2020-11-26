#pragma once
#include "sre/SpriteAtlas.hpp"

#include "PlayerObject.hpp"
#include "PhysicsComponent.hpp"
#include "PlayerPhysics.hpp"
#include "SpriteComponent.hpp"
#include "MAMGame.hpp"

PlayerObject::PlayerObject(glm::vec2 pos, sre::Sprite walk1, sre::Sprite standing, sre::Sprite walk2) : GameObject(pos, GOType::player) {
    auto pSpriteBox = addComponent<SpriteComponent>();
    this->walk1 = walk1;
    this->walk2 = walk2;
    this->standing = standing;
    pSpriteBox->setSprite(standing);
    auto phys = addComponent<PlayerPhysics>();
    b2Vec2 vertices[8];
    vertices[0].Set(-25.0f / MAMGame::instance->physicsScale,   20.0f / MAMGame::instance->physicsScale);
    vertices[1].Set(-20.0f / MAMGame::instance->physicsScale,   -20.0f / MAMGame::instance->physicsScale);
    vertices[2].Set(-10.0f / MAMGame::instance->physicsScale,   -30.0f / MAMGame::instance->physicsScale);
    vertices[3].Set(10.0f / MAMGame::instance->physicsScale,    -30.0f / MAMGame::instance->physicsScale);
    vertices[4].Set(25.0f / MAMGame::instance->physicsScale,    20.0f / MAMGame::instance->physicsScale);
    vertices[5].Set(20.0f / MAMGame::instance->physicsScale,    -20.0f / MAMGame::instance->physicsScale);
    vertices[6].Set(-10.0f / MAMGame::instance->physicsScale,   30.0f / MAMGame::instance->physicsScale);
    vertices[7].Set(10.0f / MAMGame::instance->physicsScale,    30.0f / MAMGame::instance->physicsScale);

    phys->initPolygon(b2_dynamicBody, getPosition(), 0.2f, vertices, 8, 0.2);
}

PlayerObject::~PlayerObject() {
    bow = nullptr;
}

void PlayerObject::update(float deltaTime) {
	GameObject::update(deltaTime);
    auto phys = getComponent<PhysicsComponent>();
    bool debug = false;
    if (debug) std::cout << "Begin update" << std::endl;
    if (isGrounded()) { hasCalledArrowOnceInAir = false; }
    if (movingRight && isGrounded()) {
        
        if (phys->getLinearVelocity().x < 0) {
            decelerate = true;
        }

        float xValue = 1;
        float yValue = (onRightSlope) ?  1 : 0;
        
        glm::vec2 vector = glm::vec2(xValue, yValue) * (speed * phys->getMass());
        phys->addForce(vector);
    }
    if (movingLeft && isGrounded()) {

        if (phys->getLinearVelocity().x > 0) {
            decelerate = true;
        }

        float xValue = -1;
        float yValue = (onLeftSlope) ? 1 : 0;

        glm::vec2 vector = glm::vec2(xValue, yValue) * (1000 * phys->getMass());
        phys->addForce(vector);

    }
    if (onLeftSlope && isGrounded() && groundCounter == 0 && !onRightSlope) {
        if (debug) std::cout << "standing on left" << std::endl;
        phys->addForce(glm::vec2(-1000 * phys->getMass(), 0));
    }
    if (onRightSlope && isGrounded() && groundCounter == 0 && !onLeftSlope) {
        if (debug) std::cout << "standing on right" << std::endl;
        phys->addForce(glm::vec2(1000 * phys->getMass(), 0));
    }
    if (decelerate || (!movingRight && !movingLeft && isGrounded())) {
        glm::vec2 vec = phys->getLinearVelocity();
        if (glm::length(vec) > 0.1) {
            float decel = (1 - (playerDeceleration * deltaTime));
            std::cout << "decel: " << decel << std::endl;
            phys->setLinearVelocity(vec * decel);
        }
        decelerate = false;
    }

    if (bowIsSet) {
        auto offset = flipIndicator ? glm::vec2(-15, -13) : glm::vec2(15, -13);
        auto position = getPosition() + offset;
        bow->updatePos(position);

        if (callingArrow) {
            bow->callArrow(getPosition());
            if (!isGrounded()) {
                if (bow->isHoldingArrow()) {
                    stopAfterFlying();
                    callingArrow = false;
                } else if (!hasCalledArrowOnceInAir) {
                    auto arrowPos = bow->getArrowPosition();
                    b2Vec2 toTarget = b2Vec2(arrowPos.x, arrowPos.y) - b2Vec2(getPosition().x, getPosition().y);
                    float angle = glm::radians(glm::degrees(atan2f(-toTarget.x, toTarget.y)) + 90);
                    float force = 300 * phys->getMass();
                    phys->setLinearVelocity(glm::vec2(cos(angle), sin(angle)) * force);
                    hasCalledArrowOnceInAir = true;
                }
            }
        }
    }

    updateSprite(deltaTime);
    glm::vec2 physVec = phys->getLinearVelocity();
    float length = glm::length(physVec);
    std::cout << length << std::endl;
    if (length > maxVelocity) {
        glm::vec2 newVelocity = (physVec / length) * maxVelocity;
        phys->setLinearVelocity(newVelocity);
    }
}

void PlayerObject::jump() {
    auto phys = getComponent<PhysicsComponent>();
    phys->addForce(glm::vec2(0, 22500 * phys->getMass()));
}

void PlayerObject::incrGroundCounter() {
    groundCounter++;
}

void PlayerObject::decrGroundCounter() {
    groundCounter--;
}

void PlayerObject::incrSlopeCounter() {
    slopeCounter++;
}

void PlayerObject::decrSlopeCounter() {
    slopeCounter--;
}

bool PlayerObject::isGrounded() {
    return (groundCounter + slopeCounter) != 0;
}

void PlayerObject::setOnRightSlope(bool newOnSlope) {
    onRightSlope = newOnSlope;
}
void PlayerObject::setOnLeftSlope(bool newOnSlope) {
    onLeftSlope = newOnSlope;
}

void PlayerObject::updateSprite(float deltaTime) {
    auto phys = getComponent<PhysicsComponent>();
    auto velocity = phys->getLinearVelocity();

    auto newSprite = standing;
    if (velocity.x == 0 && velocity.y == 0) {
        newSprite = standing;
        whichWalkIndicator = 2;
        walkingSpriteIndicator = 0;
    } else if (!isGrounded()) {
        newSprite = walk1;
        whichWalkIndicator = 2;
        walkingSpriteIndicator = 0;
    } else {
        if (velocity.x > 0) walkingSpriteIndicator += deltaTime * velocity.x;
        else walkingSpriteIndicator += deltaTime * velocity.x * -1;
        
        if (walkingSpriteIndicator > 1.8) {
            whichWalkIndicator = 1;
            walkingSpriteIndicator = 0;
        } else if (walkingSpriteIndicator > 1.2) {
            whichWalkIndicator = 0;
        } else if (walkingSpriteIndicator > 0.9) {
            whichWalkIndicator = 1;
        } else if (walkingSpriteIndicator > 0.3) {
            whichWalkIndicator = 2;
        }

        if (whichWalkIndicator == 0) newSprite = walk1;
        else if (whichWalkIndicator == 1) newSprite = standing;
        else if (whichWalkIndicator == 2) newSprite = walk2;
    }

    if (movingLeft) flipIndicator = true;
    else if (movingRight) flipIndicator = false;
    
    newSprite.setFlip({ flipIndicator, newSprite.getFlip().y });
    getComponent<SpriteComponent>()->setSprite(newSprite);
}

void PlayerObject::setBow(std::shared_ptr<BowObject> bow) {
    this->bow = bow;
}

void PlayerObject::pickUpBow() {
    bowIsSet = true;
}

void PlayerObject::useBow(SDL_Event& event, glm::vec2 pos) {
    if (bowIsSet) {
        bow->updateAngle(pos);
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (bow->isHoldingArrow()) {
                bow->shootArrow();
            } else {
                callingArrow = true;
            }
        } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
            if (callingArrow) {
                callingArrow = false;
                bow->stopArrow();
                if (!isGrounded() && !bow->isHoldingArrow()) {
                    stopAfterFlying();
                }
            }
        }
    }
}

void PlayerObject::stopAfterFlying() {
    auto phys = getComponent<PhysicsComponent>();
    auto currentVelocity = phys->getLinearVelocity();
    phys->setLinearVelocity(glm::vec2(0, 0));
    phys->addForce(currentVelocity * 100.0f);
}