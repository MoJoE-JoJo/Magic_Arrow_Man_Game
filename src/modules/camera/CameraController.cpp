#pragma once
#include "CameraController.hpp"
#include "../../MAMGame.hpp"
#include "sre/Camera.hpp"
#include "sre/SDLRenderer.hpp"


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

void CameraController::updateCamera(float deltaTime) {
    auto currentCameraPosition = camera.getPosition();
    auto playerPosition = MAMGame::instance->playerController->player->getPosition();

    auto easingX = 0.05f;
    auto easingY = 0.05f;
    auto newCameraPositionX = glm::mix(currentCameraPosition.x, playerPosition.x - MAMGame::instance->windowSize.x / 2, easingX);
    auto newCameraPositionY = glm::mix(currentCameraPosition.y, playerPosition.y - MAMGame::instance->windowSize.y / 2, easingY);

    auto newCameraPosition = glm::vec3((int)glm::clamp(newCameraPositionX, levelCamXMinBound, levelCamXMaxBound), (int)glm::clamp(newCameraPositionY, levelCamYMinBound, levelCamYMaxBound), camera.getPosition().z);

    camera.setPositionAndRotation(newCameraPosition, camera.getRotationEuler());
}

void CameraController::centerOnPlayer() {
    glm::vec3 playerPos = glm::vec3(MAMGame::instance->playerController->player->getPosition(), camera.getPosition().z);
    camera.setPositionAndRotation(glm::vec3(playerPos.x - MAMGame::instance->windowSize.x / 2, playerPos.y - MAMGame::instance->windowSize.y / 2, camera.getPosition().z), camera.getRotationEuler());
}


float CameraController::easingFunc(float x) {
    return 1 - (1 - x) * (1 - x);
}
