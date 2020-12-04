#pragma once
#include "sre/Camera.hpp"
#include "sre/SDLRenderer.hpp"

#include "../../MAMGame.hpp"
#include "CameraController.hpp"

void CameraController::init() {
    camera.setOrthographicProjection(MAMGame::instance->windowSize.y / 2, -1, 1);
    camera.setWindowCoordinates();
}

void CameraController::setCameraBounds(float xMin, float xMax, float yMin, float yMax) {
    levelCamXMinBound = xMin;
    levelCamXMaxBound = xMax;

    levelCamYMinBound = yMin;
    levelCamYMaxBound = yMax;
}

void CameraController::updateCamera(float deltaTime, glm::vec2 pos) {
    auto currentCameraPosition = camera.getPosition();

    auto easingX = 0.05f;
    auto easingY = 0.05f;
    auto newCameraPositionX = glm::mix(currentCameraPosition.x, pos.x - MAMGame::instance->windowSize.x / 2, easingX);
    auto newCameraPositionY = glm::mix(currentCameraPosition.y, pos.y - MAMGame::instance->windowSize.y / 2, easingY);

    auto newCameraPosition = glm::vec3((int)glm::clamp(newCameraPositionX, levelCamXMinBound, levelCamXMaxBound), (int)glm::clamp(newCameraPositionY, levelCamYMinBound, levelCamYMaxBound), camera.getPosition().z);

    camera.setPositionAndRotation(newCameraPosition, camera.getRotationEuler());
}

void CameraController::centerOnPlayer(glm::vec2 pos) {
    camera.setPositionAndRotation(glm::vec3(pos.x - MAMGame::instance->windowSize.x / 2, pos.y - MAMGame::instance->windowSize.y / 2, camera.getPosition().z), camera.getRotationEuler());
}
