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
	GameObject();														// empty constructor! Load MUST be called afterwards

	~GameObject();

	Component* FindComponent(Component_type type);
	std::vector<Component*> FindComponents(Component_type type);
	GameObject* FindChild(const char* name);

	void Update(float dt);
	void Draw();
	bool Cull(ComponentCamera* cam);

	void HierarchyTree(std::string& selected_obj_uid);

	void RayCastAgainstAABBs(Ray ray, std::list<RayHit>& outHits);		
	void RayCastAgainstMeshes(Ray ray, std::list<RayHit>& outHits);		// not recursive

	void Save(JSON_file& save_file, uint& obj_index);
	void Load(JSON_file& save_file, uint& obj_index);

	void FindParent(std::vector<GameObject*>& objects);			

public:

	GameObject* parent = nullptr;
	std::string name;						// scene saved variable
	std::string uid;						// scene saved variable
	std::string parent_uid;					// scene saved variable
	std::vector<Component*> components;		// scene saved variable
	std::vector<GameObject*> children;		// scene saved variable

	bool enabled = true;					// scene saved variable
};

