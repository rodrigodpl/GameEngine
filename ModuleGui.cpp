#include "ModuleGui.h"
#include "imgui-1.51\imgui.h"
#include "glut\glut.h"

#include <stdlib.h>

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGui::~ModuleGui(){}

bool ModuleGui::Start() {
	showdemo = true;
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;
	io.RenderDrawListsFn = nullptr;
	
	unsigned char* pixels;
	int width, height;
	SDL_Surface* font = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, (4 * width), 0, 0, 0, 0);

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	io.Fonts->TexID = (void*)SDL_CreateTextureFromSurface(SDL_GetRenderer(App->window->window), font);
<<<<<<< HEAD

=======
	
>>>>>>> 7a2cc0eeb51e9fd85f84bcf372a53efc5de37a09
	return true;
}

update_status ModuleGui::PreUpdate(float dt) {
	ImGui::NewFrame();

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		ImVec2 aux(0, 0);
		ImGui::SetNextWindowPos(aux);
		ImGui::ShowTestWindow(&showdemo);
	}	

	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate(float dt) {
	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleGui::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}