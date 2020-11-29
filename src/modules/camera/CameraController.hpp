#pragma once
#include <vector>
#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "../../GameObject.hpp"
#include "../../PlayerObject.hpp"
#include "../../../rapidjson/rapidjson.h"
#include "../../../rapidjson/document.h"

class GameObject;

class CameraController {
public:
    void init();
	void updateCamera(float deltaTime);
	void setCameraBounds(float xMin, float xMax, float yMin, float yMax);
	void centerOnPlayer();

private:
	sre::Camera camera;
	float cameraTotalMoveTime = 0.0f;
	float easingFunc(float x);
	float levelCamXMinBound;
	float levelCamXMaxBound;
	float levelCamYMinBound;
	float levelCamYMaxBound;
	
	friend class MAMGame;
};