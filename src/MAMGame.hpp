#pragma once
#include "Box2D/Dynamics/b2World.h"

#include "GameObject.hpp"
#include "PlayerController.hpp"
#include "modules/physics/components/PhysicsComponent.hpp"
#include "modules/gui/Box2DDebugDraw.hpp"
#include "modules/audio/AudioPlayer.hpp"
#include "modules/gui/Gui.hpp"
#include "modules/camera/CameraController.hpp"

enum class GameState { Running, Menu };

class MAMGame : public b2ContactListener, public b2ContactFilter {
public:
	MAMGame();
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;
	std::shared_ptr<GameObject> createGameObject(glm::vec2 pos, GOType goType);
	void createPlayerObject(glm::vec2 pos);
	void createBowObject(glm::vec2 pos, bool samePosAsPlayer);
	sre::Sprite getSprite(int index);
	void setGameState(GameState newState);
	void setGuiState(GuiState newState);
	void beginLevel(std::string filename);
	void levelWon();
	void reset();
	
	static MAMGame* instance;
	static const glm::vec2 windowSize;
	const float physicsScale = 100;
	AudioPlayer audioSystem;
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
	void startTime();
	bool isPlayerWithinBounds();
	void registerPhysicsComponent(PhysicsComponent* r);
	void deregisterPhysicsComponent(PhysicsComponent* r);

	Gui* gui;
	GuiState guiState;

	std::string currentLevel = "";
	sre::SDLRenderer r;
	b2World* world = nullptr;
	float gravity = -9.8;
	std::shared_ptr<PlayerController> playerController;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;
	Box2DDebugDraw debugDraw;
	bool doDebugDraw = false;
	std::shared_ptr<sre::SpriteAtlas> sprites;
	std::map<int, std::string> tileMap;
	GameState gameState = GameState::Menu;

	CameraController cameraController;
	float levelXMinBound;
	float levelXMaxBound;
	float levelYMinBound;
	float levelYMaxBound;

	glm::vec2 levelBounds;
	float winTimer = 0.0f;

	friend class PhysicsComponent;
};