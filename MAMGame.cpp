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

    sprites = SpriteAtlas::createSingleSprite(Texture::getWhiteTexture());

    auto box = createGameObject({ windowSize.x / 2, windowSize.y / 2 }, GOType::ground);
    auto spriteBox = box->addComponent<SpriteComponent>();
    auto sprite = sprites->get("sprite");
    sprite.setScale({ 500, 10 });
    sprite.setColor({ 0.89f, 0.55f, 0.0f, 1.0f });
    spriteBox->setSprite(sprite);

    auto phys = box->addComponent<PhysicsComponent>();
    phys->initBox(b2_staticBody, glm::vec2(500, 10) / physicsScale, box->getPosition() / physicsScale, 1);

    auto pbox = shared_ptr<PlayerObject>(new PlayerObject({ windowSize.x / 2, windowSize.y / 2 + 200 }));
    gameObjects.push_back(pbox);
    auto pSpriteBox = pbox->addComponent<SpriteComponent>();
    auto pSprite = sprites->get("sprite");
    pSprite.setScale({ 10, 10 });
    pSprite.setColor({ 0.84f, 0.27f, 0.51f, 1.0f });
    pSpriteBox->setSprite(pSprite);

    auto pPhys = pbox->addComponent<PlayerPhysics>();
    pPhys->initBox(b2_dynamicBody, glm::vec2(10, 10) / physicsScale, pbox->getPosition() / physicsScale, 0.2f);


    // Test json loading

    LevelLoader ll = LevelLoader();
    ll.loadMap("Levels/Level1.json");
    auto loaded = ll.getTileObjects();
    for each (std::shared_ptr<GameObject> ptr in loaded)
    {
        gameObjects.push_back(ptr);
    }
    playerController = shared_ptr<PlayerController>(new PlayerController(pbox));
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
    updatePhysics();

    auto it = gameObjects.begin();
    while (it != gameObjects.end()) {
        shared_ptr<GameObject> ptr = *it;
        ptr->update(time);
        it++;
    }
}

void MAMGame::updatePhysics() {
    const float32 timeStep = 1.0f / 60.0f;
    const int positionIterations = 2;
    const int velocityIterations = 6;
    world->Step(timeStep, velocityIterations, positionIterations);
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
        }
    }

    if (playerController != nullptr) {
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