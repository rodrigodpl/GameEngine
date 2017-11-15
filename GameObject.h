#pragma once

#include "Component.h"
#include "RayHit.h"
#include "MathGeoLib.h"

#include <vector>
#include <string>
#include <list>

class ComponentCamera;
class JSON_file;

class GameObject
{

public:
	GameObject(const char* name, GameObject* parent = nullptr);
	GameObject(GameObject& game_obj);

	~GameObject();

	Component* FindComponent(Component_type type);
	std::vector<Component*> FindComponents(Component_type type);
	GameObject* FindChild(const char* name);

	void UpdateRecursive(float dt);
	void DrawRecursive();
	void CullRecursive(ComponentCamera* cam);

	void CreateTree();

	void RayCastAgainstAABBs(Ray ray, std::list<RayHit>& outHits);    // recursive
	void RayCastAgainstMeshes(Ray ray, std::list<RayHit>& outHits);     // not recursive

	void Serialize(JSON_file& save_file);

public:

	GameObject* parent;
	std::string name;
	std::string uid;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

	bool culled = false;
	bool enabled = true;
};

