#pragma once
#include "GameObject.hpp"
#include "Component.hpp"

GameObject::GameObject(glm::vec2 pos) {
    position = pos;
    rotation = 0;
}

GameObject::~GameObject() {
    for (auto& c : components) {
        c->gameObject = nullptr;
    }
}

const glm::vec2& GameObject::getPosition() const {
    return position;
}

float GameObject::getRotation() const {
    return rotation;
}