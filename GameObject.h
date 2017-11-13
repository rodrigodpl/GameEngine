#pragma once

#include "Component.h"
#include "RayHit.h"
#include "MathGeoLib.h"

#include <vector>
#include <string>
#include <list>

class ComponentCamera;

class GameObject
{

public:
	GameObject(const char* name, GameObject* parent = nullptr);
	GameObject(GameObject& game_obj);

	~GameObject();

	void UpdateRecursive(float dt);
	Component* FindComponent(Component_type type);
	std::vector<Component*> FindComponents(Component_type type);
	GameObject* FindChild(const char* name);

	void DrawRecursive();
	void CreateTree();
	void CullRecursive(ComponentCamera* cam);
	void RayCastAgainstAABBs(Ray ray, std::list<RayHit>& outHits);    // recursive
	void RayCastAgainstMeshes(Ray ray, std::list<RayHit>& outHits);     // not recursive

public:

	GameObject* parent;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

	bool culled = false;

};

