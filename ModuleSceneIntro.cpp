#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "imgui.h"
#include "glut\glut.h"

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
	LOG("Loading Intro assets");
	bool ret = true;

	srand(time(NULL));

	//App->audio->PlayMusic("F-Zero_-_Mute_City.ogg");
	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;
	io.RenderDrawListsFn = nullptr;

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	SDL_Surface* font = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, (4 * width), 0, 0, 0, 0);

	io.Fonts->TexID = (void*)SDL_CreateTextureFromSurface(SDL_GetRenderer(App->window->window), font);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	ImGui::NewFrame();
	
//*	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		ImVec2 aux(0, 0);
		ImGui::SetNextWindowPos(aux);
		ImGui::ShowTestWindow();
	//}
		ImGui::Render();
	return UPDATE_CONTINUE;
}
