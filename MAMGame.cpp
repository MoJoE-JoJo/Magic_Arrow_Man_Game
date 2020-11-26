#pragma once
#include "sre/Texture.hpp"
#include "sre/SpriteAtlas.hpp"
#include "glm/gtc/random.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"

#include "MAMGame.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"
#include "PlayerPhysics.hpp"
#include "ArrowPhysics.hpp"
#include "SpriteComponent.hpp"
#include "LevelLoader.hpp"

using namespace sre;
using namespace std;

MAMGame* MAMGame::instance = nullptr;
const glm::vec2 MAMGame::windowSize(1280, 720);

MAMGame::MAMGame() : debugDraw(physicsScale) {
    instance = this;
    r.setWindowSize(windowSize);
    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING).withSdlWindowFlags(SDL_WINDOW_OPENGL);
    createTileMap();
    init();

    gui = new Gui(windowSize);

    // setup callback functions
    r.keyEvent = [&](SDL_Event& e) { onKey(e); };
    r.mouseEvent = [&](SDL_Event& e) { mouseEvent(e); };
    r.frameUpdate = [&](float deltaTime) { update(deltaTime); };
    r.frameRender = [&]() { render(); };
    // start game loop
    r.startEventLoop();
}

void MAMGame::init() {
    if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world
        world->SetContactListener(nullptr);
        world->SetContactFilter(nullptr);
    }

    playerController = nullptr;
    gameObjects.clear();
    physicsComponentLookup.clear();

    camera.setOrthographicProjection(windowSize.y / 2, -1, 1);
    camera.setWindowCoordinates();
    initPhysics();

    sprites = SpriteAtlas::create("MAM.json", "MAM.png");
    // Test json loading
    LevelLoader ll = LevelLoader();
    ll.loadMap("Levels/Level" + std::to_string(currentLevel) + ".json");
    levelBounds = glm::vec2(ll.getMapWidth(), ll.getMapHeight());
    
    levelCamXMinBound = 0.0f - 64.0f; //-64.0f Keeps half a tile of padding at the left side of the screeen
    levelCamXMaxBound = levelBounds.x - windowSize.x - 0.0f; //-0.0f Keeps half a tile of padding at the right side of the screeen

    levelCamYMinBound = -levelBounds.y + 0.0f; //+0.0f Keeps half a tile of padding at the bottom of the screeen
    levelCamYMaxBound = 0.0f - windowSize.y + 64.0f; //+64.0f Keeps half a tile of padding at the top of the screeen

    levelXMinBound = 0.0f - 64.0f;
    levelXMaxBound = levelBounds.x;
    levelYMinBound = -levelBounds.y;
    levelYMaxBound = 0.0f + 64.0f;
}

void MAMGame::initPhysics() {
    delete world;
    world = new b2World(b2Vec2(0, gravity));
    world->SetContactListener(this);
    world->SetContactFilter(this);
    if (doDebugDraw) {
        world->SetDebugDraw(&debugDraw);
    }
}

std::shared_ptr<GameObject> MAMGame::createGameObject(glm::vec2 pos, GOType goType) {
    auto obj = shared_ptr<GameObject>(new GameObject(pos, goType));
    gameObjects.push_back(obj);
    return obj;
}

void MAMGame::createPlayerObject(glm::vec2 pos) {
    auto player = shared_ptr<PlayerObject>(new PlayerObject(pos, sprites->get("player_f1.png"), sprites->get("player_f2.png"), sprites->get("player_f3.png")));
    gameObjects.push_back(player);
    playerController = shared_ptr<PlayerController>(new PlayerController(player));
    camera.setPositionAndRotation(glm::vec3(pos.x - windowSize.x / 2, pos.y - windowSize.y / 2, camera.getPosition().z), camera.getRotationEuler());
}

void MAMGame::createBowObject(glm::vec2 pos, bool samePosAsPlayer) {
    auto arrow = createGameObject(pos, GOType::arrow);
    auto arrowSpriteBox = arrow->addComponent<SpriteComponent>();
    arrowSpriteBox->setSprite(sprites->get("blue_arrow_projectile.png"));
    auto phys = arrow->addComponent<ArrowPhysics>();
    phys->initArrow(b2_dynamicBody, arrow->getPosition() * 100000.0f);

    auto bow = shared_ptr<BowObject>(new BowObject(pos, sprites->get("bow.png"), arrow));
    gameObjects.push_back(bow);
    playerController->player->setBow(bow, samePosAsPlayer);
}

void MAMGame::BeginContact(b2Contact* contact) {
    b2ContactListener::BeginContact(contact);
    handleContact(contact, true);
}

void MAMGame::EndContact(b2Contact* contact) {
    b2ContactListener::EndContact(contact);
    handleContact(contact, false);
}

void MAMGame::handleContact(b2Contact* contact, bool begin) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto physA = physicsComponentLookup.find(fixA);
    auto physB = physicsComponentLookup.find(fixB);
    if (physA != physicsComponentLookup.end() && physB != physicsComponentLookup.end()) {
        if (begin) {
            physA->second->onCollisionStart(physB->second);
            physB->second->onCollisionStart(physA->second);
        } else {
            physA->second->onCollisionEnd(physB->second);
            physB->second->onCollisionEnd(physA->second);
        }
    }
}

bool MAMGame::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) {
    auto physA = physicsComponentLookup.find(fixtureA);
    auto physB = physicsComponentLookup.find(fixtureB);
    if (physA != physicsComponentLookup.end() && physB != physicsComponentLookup.end()) {
        if (physA->second->getGameObject()->goType == GOType::arrow && physB->second->getGameObject()->goType == GOType::player) {
            return false;
        } else if (physB->second->getGameObject()->goType == GOType::arrow && physA->second->getGameObject()->goType == GOType::player) {
            return false;
        }
    }
    return true;
}

void MAMGame::update(float time) {
    if (gameState == GameState::Running) {
        updatePhysics();
        
        auto it = gameObjects.begin();
        while (it != gameObjects.end()) {
            shared_ptr<GameObject> ptr = *it;
            ptr->update(time);
            it++;
        }
        if (!isPlayerWithinBounds()) reset();

        updateCamera(time);  
    }
}

void MAMGame::updatePhysics() {
    const float32 timeStep = 1.0f / 60.0f;
    const int positionIterations = 2;
    const int velocityIterations = 6;
    world->Step(timeStep, velocityIterations, positionIterations);
}

void MAMGame::updateCamera(float time) {
    auto currentCameraPosition = camera.getPosition();
    auto playerPosition = playerController->player->getPosition();

    auto easingX = 0.035f;
    auto easingY = 0.035f;
    auto newCameraPositionX = glm::mix(currentCameraPosition.x, playerPosition.x - windowSize.x / 2, easingX);
    auto newCameraPositionY = glm::mix(currentCameraPosition.y, playerPosition.y - windowSize.y / 2, easingY);
    
    auto newCameraPosition = glm::vec3((int) glm::clamp(newCameraPositionX, levelCamXMinBound, levelCamXMaxBound), (int) glm::clamp(newCameraPositionY, levelCamYMinBound, levelCamYMaxBound), camera.getPosition().z);
    
    camera.setPositionAndRotation(newCameraPosition, camera.getRotationEuler());
}

float MAMGame::easingFunc(float x) {
    return 1 - (1 - x) * (1 - x);
}

void MAMGame::render() {
    auto rp = RenderPass::create().withCamera(camera).withClearColor(true, { .52f, .80f, .92f, 1.0f }).build();

    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto& go : gameObjects) {
        auto spriteComponent = go->getComponent<SpriteComponent>();
        if (spriteComponent != nullptr) {
            spriteComponent->renderSprite(spriteBatchBuilder);
        }
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);

    if (doDebugDraw) {
        world->DrawDebugData();
        rp.drawLines(debugDraw.getLines());
        debugDraw.clear();
    }

    if (gameState == GameState::Menu) {
        gui->renderGui();
    }
}

void MAMGame::onKey(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_p) {
            doDebugDraw = !doDebugDraw;
            if (doDebugDraw) {
                world->SetDebugDraw(&debugDraw);
            } else {
                world->SetDebugDraw(nullptr);
            }
        } else if (event.key.keysym.sym == SDLK_r) {
            reset();
            return;
        } else if (event.key.keysym.sym == SDLK_q) {
            setGameState(GameState::Menu);
        } else if (event.key.keysym.sym == SDLK_SPACE && gameState == GameState::Won) {
            beginLevel(currentLevel + 1);
            return;
        }
    }

    if (playerController != nullptr && gameState != GameState::Won) {
        playerController->onKey(event);
    }
}

void MAMGame::mouseEvent(SDL_Event& e) {
    auto r = Renderer::instance;
    glm::vec2 pos{ e.motion.x, r->getWindowSize().y - e.motion.y };

    // convert to pixel coordinates (for HighDPI displays)
    pos /= r->getWindowSize();
    pos *= r->getDrawableSize();

    playerController->mouseEvent(e, glm::vec2(camera.screenPointToRay(pos)[0]));
}

void MAMGame::registerPhysicsComponent(PhysicsComponent* physComponent) {
    physicsComponentLookup[physComponent->fixture] = physComponent;
}

void MAMGame::deregisterPhysicsComponent(PhysicsComponent* physComponent) {
    auto iter = physicsComponentLookup.find(physComponent->fixture);
    if (iter != physicsComponentLookup.end()) {
        physicsComponentLookup.erase(iter);
    } else {
        assert(false); // cannot find physics object
    }
}

sre::Sprite MAMGame::getSprite(int index) {
    return sprites->get(tileMap.find(index)->second);
}

void MAMGame::setGameState(GameState newState) {
    gameState = newState;
}

void MAMGame::createTileMap() {
    tileMap.insert({ 2, "Tilesets-1-01.png" });
    tileMap.insert({ 3, "Tilesets-1-02.png" });
    tileMap.insert({ 4, "Tilesets-1-03.png" });
    tileMap.insert({ 5, "Tilesets-1-04.png" });
    tileMap.insert({ 6, "Tilesets-1-05.png" });
    tileMap.insert({ 7, "Tilesets-1-06.png" });
    tileMap.insert({ 8, "Tilesets-1-07.png" });
    tileMap.insert({ 9, "Tilesets-1-08.png" });
    tileMap.insert({ 10, "Tilesets-1-09.png" });
    tileMap.insert({ 11, "Tilesets-1-10.png" });
    tileMap.insert({ 12, "Tilesets-1-11.png" });
    tileMap.insert({ 13, "Tilesets-1-12.png" });
    tileMap.insert({ 14, "Tilesets-1-13.png" });
    tileMap.insert({ 15, "Tilesets-1-14.png" });
    tileMap.insert({ 16, "Tilesets-1-15.png" });
    tileMap.insert({ 17, "Tilesets-1-16.png" });
    tileMap.insert({ 18, "Tilesets-1-17.png" });
    tileMap.insert({ 19, "Tilesets-1-18.png" });
    tileMap.insert({ 20, "spawn.png" }); // Not an actual number
    tileMap.insert({ 21, "monster_f1.png" });
    tileMap.insert({ 22, "target.png" });
    tileMap.insert({ 23, "target_legs.png" });
    tileMap.insert({ 24, "bow.png" });
}

void MAMGame::reset() {
    playerController->reset();
}

bool MAMGame::isPlayerWithinBounds() {
    auto playerPosition = playerController->player->getPosition();
    if (playerPosition.x < levelXMinBound) return false;
    if (playerPosition.x > levelXMaxBound) return false;
    if (playerPosition.y < levelYMinBound) return false;
    if (playerPosition.y > levelYMaxBound) return false;
    return true;
}

void MAMGame::beginLevel(int level) {
    currentLevel = level;
    init();
    setGameState(GameState::Running);
}