#pragma once
#include "sre/SDLRenderer.hpp"
#include "Box2D/Dynamics/b2World.h"
#include "sre/Camera.hpp"

#include "GameObject.hpp"

class MAMGame : public b2ContactListener {
public:
	MAMGame();
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	std::shared_ptr<GameObject> createGameObject(glm::vec2 pos);

	static MAMGame* instance;
	static const glm::vec2 windowSize;
private:
	void init();
	void initPhysics();
	void update(float time);
	void render();
	void onKey(SDL_Event& event);
	void mouseEvent(SDL_Event& event);
	void handleContact(b2Contact* contact, bool begin);

	sre::SDLRenderer r;
	b2World* world = nullptr;
	float gravity = -9.8;
	sre::Camera camera;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};