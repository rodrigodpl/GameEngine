#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

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
	App->gui->app_log.AddLog("Loading Intro assets");
	bool ret = true;

	srand(time(NULL));
	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	game_objects.push_back(App->importer->LoadFBX("../Test_files/BakerHouse.fbx"));
	//App->importer->LoadFBX("Test_files/Spaceship.fbx");

	return ret;

}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->gui->app_log.AddLog("Unloading Intro scene\n");
	materials.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawScene()
{
	for (std::list<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		(*it)->DrawRecursive();		
}
