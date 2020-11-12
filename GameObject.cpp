#pragma once
#include "GameObject.hpp"
#include "Component.hpp"

GameObject::GameObject(glm::vec2 pos, GOType goType) {
    position = pos;
    rotation = 0;
    this->goType = goType;
}

GameObject::~GameObject() {
    for (auto& c : components) {
        c->gameObject = nullptr;
    }
}

const glm::vec2& GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const glm::vec2& position) {
    GameObject::position = position;
}

float GameObject::getRotation() const {
    return rotation;
}

void GameObject::setRotation(float rotation) {
    GameObject::rotation = rotation;
}

void GameObject::update(float deltaTime) {
    for (auto& comp : components) {
        comp->update(deltaTime);
    }
}