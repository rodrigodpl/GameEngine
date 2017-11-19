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
	void AddRootObject(GameObject* game_obj);		// utility to add objects to both root_objects and all_objects vector
	GameObject* GetSelectedObj();
public:

	std::vector<GameObject*> all_game_objects;
	std::vector<GameObject*> root_game_objects;		// for recursive functions
	std::string selected_game_obj_uid;

	std::vector<ComponentMaterial*> materials;

	// config saved variables
	std::string name = "main_scene.json";

};
