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
	/*ImGuiStyle * style = &ImGui::GetStyle();
	style->Alpha = 1.0f;*/

	show_menu = true;
	showdemo = false;
	show_about = false;

	return true;
}

update_status ModuleGui::PreUpdate(float dt) {
	//  Main Menu -------
	ImGui_ImplSdl_NewFrame(App->window->window);
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File", &show_menu))
		{
			if (ImGui::MenuItem("Exit")) {
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("ShowAbout")) {
				ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
				ImGui::Begin("About", &show_about, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
				ImGui::Text("Space Engine");
				ImGui::Text("It renders in parseks");
				ImGui::Separator();
				ImGui::Text("By Rodrigo de Pedro & Daniel Garcia");
				ImGui::End();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	// ------------------

	// Test window ------
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		showdemo = !showdemo;

	if (showdemo)
		ImGui::ShowTestWindow(&showdemo);
	// ------------------

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

void ModuleGui::showaboutmenu() {
	
}