#pragma once
#include "Module.h"

#include <vector>
#include <string>

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
	
	void Save(JSON_file& config);
	void Load(JSON_file& config);

	void DrawScene();
public:

	std::vector<GameObject*> game_objects;
	GameObject* selected_game_obj = nullptr;

	std::vector<ComponentMaterial*> materials;

	// config saved variables
	std::string name = "main_scene.json";

};
