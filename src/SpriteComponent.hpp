#pragma once
#include "sre/Sprite.hpp"
#include "sre/SpriteBatch.hpp"

#include "Component.hpp"

class SpriteComponent : public Component {
public:
    explicit SpriteComponent(GameObject* gameObject);
    void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);
    void setSprite(const sre::Sprite& sprite);
    void update(float deltaTime) override;
private:
    sre::Sprite sprite;
};