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

	AddCube(2, 2, 2);
	AddSphere(2);
	AddCylinder(2, 2);
	AddLine({ 0,0,0 }, { 2,2,2 });

	return ret;

}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->gui->app_log.AddLog("Unloading Intro scene\n");
	primitives.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		ShowPrimitives(PrimitiveTypes::Primitive_Cube);
	else if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		ShowPrimitives(PrimitiveTypes::Primitive_Sphere);
	else if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
		ShowPrimitives(PrimitiveTypes::Primitive_Cylinder);
	else if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
		ShowPrimitives(PrimitiveTypes::Primitive_Line);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::ShowPrimitives(PrimitiveTypes type) {

	for (std::list<Primitive*>::iterator it = primitives.begin(); it != primitives.end(); it++) {

		if ((*it)->type == type)
			(*it)->visible = true;
		else
			(*it)->visible = false;
	}

	for (std::list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); it++)
		(*it).visible = false;

}

void ModuleSceneIntro::ShowNextMesh() {

	for (std::list<Primitive*>::iterator it = primitives.begin(); it != primitives.end(); it++) 
			(*it)->visible = false;
	

	for (std::list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); it++)
		(*it).visible = false;

}

void ModuleSceneIntro::Draw() 
{
	for (std::list<Primitive*>::iterator it = primitives.begin(); it != primitives.end(); it++) {

		if((*it)->visible)
			(*it)->Render();
	}

	for (std::list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); it++) {

		if((*it).visible)
			App->renderer3D->DrawMesh(*it);
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

void ModuleSceneIntro::AddLine(Vertex origin, Vertex dest) 
{
	primitives.push_back(new Line(origin, dest));
}