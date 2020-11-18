#pragma once
#include <memory>
#include <vector>
#include <string>
#include "glm/glm.hpp"

class Component;
enum class GOType { player, ground, wall, slope, target };

class GameObject {
public:
    GameObject(glm::vec2 pos, GOType goType);
    ~GameObject();
    
    float getRotation() const;
    void setRotation(float rotation);
    const glm::vec2& getPosition() const;
    void setPosition(const glm::vec2& position);
    virtual void update(float deltaTime);

    GOType goType;

    template <class T>
        std::shared_ptr<T> addComponent();
    template <class T>
        std::shared_ptr<T> getComponent();
private:
    std::vector<std::shared_ptr<Component>> components;
    glm::vec2 position;
    float rotation;
};

template <class T>
inline std::shared_ptr<T> GameObject::addComponent() {
    auto obj = std::shared_ptr<T>(new T(this));
    components.push_back(obj);

    return obj;
}

template <class T>
inline std::shared_ptr<T> GameObject::getComponent() {
    for (auto c : components) {
        std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(c);
        if (res != nullptr) {
            return res;
        }
    }
    return std::shared_ptr<T>();
}