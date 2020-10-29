#pragma once
#include "sre/Texture.hpp"
#include "sre/SpriteAtlas.hpp"
#include "glm/gtc/random.hpp"

#include "MAMGame.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"

using namespace sre;
using namespace std;

MAMGame* MAMGame::instance = nullptr;
const glm::vec2 MAMGame::windowSize(1280, 720);

MAMGame::MAMGame() {
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
    camera.setOrthographicProjection(windowSize.y / 2, -1, 1);
    camera.setWindowCoordinates();
    initPhysics();

    auto sprites = SpriteAtlas::createSingleSprite(Texture::getWhiteTexture());
    auto box = createGameObject({ windowSize.x / 2, windowSize.y / 2 });
    auto spriteBox = box->addComponent<SpriteComponent>();
    auto sprite = sprites->get("sprite");
    sprite.setScale({ 100, 10 });
    sprite.setColor({ 0.89f, 0.55f, 0.0f, 1.0f });
    spriteBox->setSprite(sprite);
}

void MAMGame::initPhysics() {
    delete world;
    world = new b2World(b2Vec2(0, gravity));
    world->SetContactListener(this);
}

void MAMGame::BeginContact(b2Contact* contact) {
    b2ContactListener::BeginContact(contact);
    handleContact(contact, true);
}

void MAMGame::EndContact(b2Contact* contact) {
    b2ContactListener::EndContact(contact);
    handleContact(contact, false);
}

std::shared_ptr<GameObject> MAMGame::createGameObject(glm::vec2 pos) {
    auto obj = shared_ptr<GameObject>(new GameObject(pos));
    gameObjects.push_back(obj);
    return obj;
}

void MAMGame::handleContact(b2Contact* contact, bool begin) {

}

void MAMGame::update(float time) {
    
}

void MAMGame::render() {
    auto rp = RenderPass::create()
        .withCamera(camera)
        .withClearColor(true, { .52f, .80f, .92f, 1.0f })
        .build();

    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto& go : gameObjects) {
        auto spriteComponent = go->getComponent<SpriteComponent>();
        if (spriteComponent != nullptr) {
            spriteComponent->renderSprite(spriteBatchBuilder);
        }
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);
}

void MAMGame::onKey(SDL_Event& event) {

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