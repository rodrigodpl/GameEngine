#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include <stdlib.h>
#include <time.h>

#define RADIUS 44

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

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

	AddCylinder(1, 2);
	return ret;

}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->gui->app_log.AddLog("Unloading Intro scene\n");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::Draw() 
{
	for (std::list<Primitive*>::iterator it = primitives.begin(); it != primitives.end(); it++) {

		if((*it)->visible)
			(*it)->Render();
	}
		
}

void ModuleSceneIntro::AddCube(float size_x, float size_y, float size_z) 
{
	primitives.push_back(new Cube(size_x, size_y, size_z));
}


void ModuleSceneIntro::AddSphere(float radius, uint rings, uint sectors) 
{
	primitives.push_back(new Sphere(radius, rings, sectors));
}

void ModuleSceneIntro::AddCylinder(float radius, float height, uint sides) 
{
	primitives.push_back(new Cylinder(radius, height, sides));
}