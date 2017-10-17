#pragma once

#include "Component.h"

#include <vector>
#include <string>

class GameObject
{

public:
	GameObject();
	GameObject(GameObject& game_obj);

	~GameObject();

	void Update(float dt);
	Component* FindComponent(Component_type type);

public:

	GameObject* parent;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

};
