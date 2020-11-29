#pragma once
#include "../../../rapidjson/rapidjson.h"
#include "../../../rapidjson/document.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include <memory>

#include "../../GameObject.hpp"
#include "../../PlayerObject.hpp"

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
	float levelCamXMinBound;
	float levelCamXMaxBound;
	float levelCamYMinBound;
	float levelCamYMaxBound;
	
	friend class MAMGame;
};