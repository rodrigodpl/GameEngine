#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "GameObject.h"

#include <stdlib.h>
#include <time.h>

#define RADIUS 44

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	selected_game_obj_uid.append("none");
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->gui->app_log.AddLog("Loading Intro assets\n");

	srand(time(NULL));

	if (empty_scene) App->importer->LoadAssets(App->fs->GetFilesInDir(ASSETS_BASE_PATH));

	GameObject* floor = new GameObject("floor plane");
	floor->components.push_back((Component*)new PrimPlane(0, 1, 0, 10));
	App->scene_intro->AddRootObject(floor);

	return true;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->gui->app_log.AddLog("Unloading Intro scene\n");
	materials.clear();
	all_game_objects.clear();
	root_game_objects.clear();

	return true;
}

void ModuleSceneIntro::Save(JSON_file& config)
{
	JSON_file* save_file = App->json->OpenFile(name.c_str(), ASSETS_BASE_PATH);

	save_file->WriteBool("empty", false);
	save_file->WriteNumber("obj_num", all_game_objects.size());

	uint obj_index = 0;
	for (std::vector<GameObject*>::iterator it = all_game_objects.begin(); it != all_game_objects.end(); it++, obj_index++)
		(*it)->Save(*save_file, obj_index);

	save_file->Save();
	
	config.WriteString("scene_manager.current_scene", name.c_str());
}

void ModuleSceneIntro::Load(JSON_file& config)
{
	name = config.ReadString("scene_manager.current_scene");

	JSON_file* save_file = App->json->OpenFile(name.c_str(), ASSETS_BASE_PATH);
	
	if (!save_file->ReadBool("empty"))
	{
		empty_scene = false;
		uint obj_num = save_file->ReadNumber("obj_num");
		for (uint obj_index = 0; obj_index < obj_num; obj_index++)
		{
			GameObject* new_obj = new GameObject();
			new_obj->Load(*save_file, obj_index);

			if (new_obj->parent_uid == "none")	AddRootObject(new_obj);
			else								all_game_objects.push_back(new_obj);
		}

		for (std::vector<GameObject*>::iterator it = all_game_objects.begin(); it != all_game_objects.end(); it++)
			(*it)->FindParent(all_game_objects);
	}
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawScene()
{
	for (std::vector<GameObject*>::iterator it = all_game_objects.begin(); it != all_game_objects.end(); it++) {
		if(!(*it)->Cull(App->camera->cam)) 
			(*it)->Draw();
	}
}

void ModuleSceneIntro::AddRootObject(GameObject* game_obj)
{
	root_game_objects.push_back(game_obj);
	all_game_objects.push_back(game_obj);
}

