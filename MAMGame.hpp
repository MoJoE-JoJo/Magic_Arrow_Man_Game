#pragma once
#include "sre/SDLRenderer.hpp"
#include "Box2D/Dynamics/b2World.h"
#include "sre/Camera.hpp"

#include "GameObject.hpp"
#include "PlayerController.hpp"
#include "PhysicsComponent.hpp"
#include "Box2DDebugDraw.hpp"

enum class GameState { Won, Running };

class MAMGame : public b2ContactListener {
public:
	MAMGame();
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	std::shared_ptr<GameObject> createGameObject(glm::vec2 pos, GOType goType);
	void createPlayerObject(glm::vec2 pos);
	void createBowObject(glm::vec2 pos);
	void setBow();
	static MAMGame* instance;
	static const glm::vec2 windowSize;
	sre::Sprite getSprite(int index);
	void setGameState(GameState newState);
	
	const float physicsScale = 100;
private:
	void init();
	void initPhysics();
	void update(float time);
	void updatePhysics();
	void render();
	void onKey(SDL_Event& event);
	void mouseEvent(SDL_Event& event);
	void handleContact(b2Contact* contact, bool begin);
	void createTileMap();

	int currentLevel = 0;
	sre::SDLRenderer r;
	b2World* world = nullptr;
	float gravity = -9.8;
	sre::Camera camera;
	std::shared_ptr<PlayerController> playerController;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	void registerPhysicsComponent(PhysicsComponent* r);
	void deregisterPhysicsComponent(PhysicsComponent* r);
	std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;
	Box2DDebugDraw debugDraw;
	bool doDebugDraw = false;
	std::shared_ptr<sre::SpriteAtlas> sprites;
	std::map<int, std::string> tileMap;
	GameState gameState = GameState::Running;
	friend class PhysicsComponent;
	friend class LevelLoader;
};