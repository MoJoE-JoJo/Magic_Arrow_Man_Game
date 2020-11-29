#pragma once

class GameObject;

class Component {
public:
	explicit Component(GameObject* gameObject);
	virtual ~Component() = default;
	virtual void update(float deltaTime);
	GameObject* getGameObject();
protected:
	GameObject* gameObject;
	friend class GameObject;
};