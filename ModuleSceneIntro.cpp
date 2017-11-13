#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "GameObject.h"

#include <stdlib.h>
#include <time.h>

#define RADIUS 44

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->gui->app_log.AddLog("Loading Intro assets\n");
	bool ret = true;

	srand(time(NULL));

	game_objects.push_back(App->importer->LoadFBX("../Assets/BakerHouse.fbx"));
	//App->importer->LoadFBX("Test_files/Spaceship.fbx");

	GameObject* floor = new GameObject("floor plane");
	floor->components.push_back((Component*)new PrimPlane(0, 1, 0, 10));
	game_objects.push_back(floor);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->gui->app_log.AddLog("Unloading Intro scene\n");
	materials.clear();
	game_objects.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawScene()
{
	for (std::list<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++) {
		(*it)->CullRecursive(App->camera->cam);
		(*it)->DrawRecursive();
	}
}

