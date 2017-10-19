#pragma once

#include "Component.h"

#include <vector>
#include <string>

class GameObject
{

public:
	GameObject(const char* name, GameObject* parent = nullptr);
	GameObject(GameObject& game_obj);

	~GameObject();

	void Update(float dt);
	Component* FindComponent(Component_type type);
	GameObject* FindChild(const char* name);

	void DrawRecursive();

public:

	GameObject* parent;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

};
