#pragma once
#include "sre/Texture.hpp"
#include "sre/SpriteAtlas.hpp"
#include "glm/gtc/random.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"

#include "MAMGame.hpp"
#include "GameObject.hpp"
#include "PlayerObject.hpp"
#include "PlayerPhysics.hpp"
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
}

void MAMGame::initPhysics() {
    delete world;
    world = new b2World(b2Vec2(0, gravity));
    world->SetContactListener(this);
    if (doDebugDraw) {
        world->SetDebugDraw(&debugDraw);
    }
}

void MAMGame::BeginContact(b2Contact* contact) {
    b2ContactListener::BeginContact(contact);
    handleContact(contact, true);
}

void MAMGame::EndContact(b2Contact* contact) {
    b2ContactListener::EndContact(contact);
    handleContact(contact, false);
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
    // TODO: remove when camera works properly
    camera.setPositionAndRotation(glm::vec3(pos.x - windowSize.x / 2, pos.y - windowSize.y / 2, camera.getPosition().z), camera.getRotationEuler());
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

void MAMGame::update(float time) {
    if (gameState == GameState::Running) {
        updatePhysics();
        
        auto it = gameObjects.begin();
        while (it != gameObjects.end()) {
            shared_ptr<GameObject> ptr = *it;
            ptr->update(time);
            it++;
        }
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
    //auto currentCameraRotation = camera.getRotationEuler();

    //float sTime = time;

    //float moveSpeed = 5.0f;
    //cameraTotalMoveTime += sTime;
    auto playerPosition = playerController->player->getPosition();
    //cameraTotalMoveTime = (float)fmod(cameraTotalMoveTime, 1);

    //std::cout << cameraTotalMoveTime << endl;


    //auto cameraPlayerDiff = glm::vec3(playerPosition.x - windowSize.x / 2, playerPosition.y - windowSize.y / 2, camera.getPosition().z) - currentCameraPosition;
    auto easingX = 0.035f;
    auto easingY = 0.035f;
    //auto easingX = easingFunc(0.01f);
    //auto easingY = easingFunc(0.01f);
    auto newCameraPositionX = glm::mix(currentCameraPosition.x, playerPosition.x - windowSize.x / 2, easingX);
    auto newCameraPositionY = glm::mix(currentCameraPosition.y, playerPosition.y - windowSize.y / 2, easingY);
    
    auto xMin = 0.0f - 64.0f; //-64.0f Keeps half a tile of padding at the left side of the screeen
    auto xMax = levelBounds.x - windowSize.x - 0.0f; //-0.0f Keeps half a tile of padding at the right side of the screeen

    auto yMin = - levelBounds.y + 0.0f; //+0.0f Keeps half a tile of padding at the bottom of the screeen
    auto yMax = 0.0f - windowSize.y + 64.0f; //+64.0f Keeps half a tile of padding at the top of the screeen

    auto newCameraPosition = glm::vec3((int) glm::clamp(newCameraPositionX, xMin, xMax), (int) glm::clamp(newCameraPositionY, yMin, yMax), camera.getPosition().z);
    
    camera.setPositionAndRotation(newCameraPosition, camera.getRotationEuler());
}

float MAMGame::easingFunc(float x) {
    //return x;
    return 1 - (1 - x) * (1 - x);
    //return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
    //return x * x;
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
}

void MAMGame::onKey(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_q) {
            doDebugDraw = !doDebugDraw;
            if (doDebugDraw) {
                world->SetDebugDraw(&debugDraw);
            } else {
                world->SetDebugDraw(nullptr);
            }
        } else if (event.key.keysym.sym == SDLK_r) {
            init();
            return;
        } else if (event.key.keysym.sym == SDLK_LEFT) {
            camera.setPositionAndRotation(glm::vec3(camera.getPosition().x - 20, camera.getPosition().y, camera.getPosition().z), camera.getRotationEuler());
        } else if (event.key.keysym.sym == SDLK_RIGHT) {
            camera.setPositionAndRotation(glm::vec3(camera.getPosition().x + 20, camera.getPosition().y, camera.getPosition().z), camera.getRotationEuler());
        } else if (event.key.keysym.sym == SDLK_UP) {
            camera.setPositionAndRotation(glm::vec3(camera.getPosition().x, camera.getPosition().y + 20, camera.getPosition().z), camera.getRotationEuler());
        } else if (event.key.keysym.sym == SDLK_DOWN) {
            camera.setPositionAndRotation(glm::vec3(camera.getPosition().x, camera.getPosition().y - 20, camera.getPosition().z), camera.getRotationEuler());
        } else if (event.key.keysym.sym == SDLK_SPACE && gameState == GameState::Won) {
            currentLevel++;
            init();
            setGameState(GameState::Running);
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

    int mouseX = static_cast<int>(pos.x);
    int mouseY = static_cast<int>(pos.y);

    std::cout << mouseX << " " << mouseY << std::endl;
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
}