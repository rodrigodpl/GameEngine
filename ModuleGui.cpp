#include "ModuleGui.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "Application.h"

#include <stdlib.h>

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGui::~ModuleGui(){}

bool ModuleGui::Start() {

	ImGui_ImplSdl_Init(App->window->window);

	return true;
}

update_status ModuleGui::PreUpdate(float dt) {

	ImGui_ImplSdl_NewFrame(App->window->window);

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		showdemo = !showdemo;

	if(showdemo)
		ImGui::ShowTestWindow(&showdemo);
	

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