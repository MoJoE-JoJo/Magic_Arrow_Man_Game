#pragma once
#include "Component.hpp"

Component::Component(GameObject* gameObject) : gameObject(gameObject) { }

GameObject* Component::getGameObject() {
    return gameObject;
}

void Component::update(float deltaTime) { }