#pragma once
#include "Module.h"

#include <list>
#include <vector>

class ComponentMaterial;
class GameObject;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void DrawScene();
public:

	std::list<GameObject*> game_objects;

	std::vector<ComponentMaterial*> materials;

};
