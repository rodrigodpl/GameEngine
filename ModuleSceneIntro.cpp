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
	name = "main_scene.json";
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->gui->app_log.AddLog("Loading Intro assets\n");

	srand(time(NULL));

	GameObject* floor = new GameObject("floor plane");
	floor->components.push_back((Component*)new PrimPlane(0, 1, 0, 10));
	game_objects.push_back(floor);

	return true;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->gui->app_log.AddLog("Unloading Intro scene\n");
	materials.clear();
	game_objects.clear();

	return true;
}

// load: name = "main_scene.json";

void ModuleSceneIntro::Save(JSON_file& config)
{
	JSON_file* save_file = App->json->OpenFile(name.c_str(), LIBRARY_BASE_PATH);

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		(*it)->Serialize(*save_file);

	save_file->Save();
	
	config.WriteString("scene_manager.current_scene", name.c_str());
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawScene()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++) {
		(*it)->CullRecursive(App->camera->cam);
		(*it)->DrawRecursive();
	}
}

