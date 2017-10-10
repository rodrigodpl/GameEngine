#include "ModuleGui.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "Globals.h"
#include "Application.h"

#include <stdlib.h>

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGui::~ModuleGui() {}

bool ModuleGui::Start() {

	ImGui_ImplSdl_Init(App->window->window);

	return true;

}

update_status ModuleGui::PreUpdate(float dt) {


	//  Main Menu -------
	ImGui_ImplSdl_NewFrame(App->window->window);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File", &draw_menu))
		{
			if (ImGui::MenuItem("Exit")) {
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Hardware"))
				draw_hardware = !draw_hardware;
			if (ImGui::MenuItem("Console"))
				draw_log = !draw_log;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
				draw_about = !draw_about;

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// ------------------


	// Test window ------
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		draw_demo = !draw_demo;

	return UPDATE_CONTINUE;
}


bool ModuleGui::CleanUp()
{
	App->gui->app_log.AddLog("Unloading Intro scene\n");
	return true;
}

void ModuleGui::HandleEvent(SDL_Event* sdl_event)
{
	ImGui_ImplSdl_ProcessEvent(sdl_event);
}

void ModuleGui::Draw() {


	if (draw_demo)
		ImGui::ShowTestWindow(&draw_demo);

	if (draw_log)
		app_log.Draw("Log", &draw_log);

	if (draw_hardware) {
		ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Hardware", &draw_hardware, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
		ImGui::Text("CPU architecture: x%d", SDL_GetCPUCacheLineSize());
		ImGui::Separator();
		ImGui::Text("System RAM: %d MB", SDL_GetSystemRAM());
		ImGui::End();
	}

	if (draw_about) {
		ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("About", &draw_about, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
		ImGui::Text("Space Engine");
		ImGui::Text("It renders in parseks");
		ImGui::Separator();
		ImGui::Text("By Rodrigo de Pedro & Daniel Garcia");
		ImGui::Separator();
		ImGui::Text("Made with:");
		ImGui::Text("Brofiler      v.1.1.1");
		ImGui::Text("Bullet        v.2.84");
		ImGui::Text("Imgui         v.1.51");
		ImGui::Text("MathGeoLib    v.1.51");
		ImGui::End();
	}

	ImGui::Render();
}


void AppLog::Clear() { Buf.clear(); LineOffsets.clear(); }

void AppLog::AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
{

	int old_size = Buf.size();
	va_list args;
	va_start(args, fmt);
	Buf.appendv(fmt, args);
	va_end(args);
	for (int new_size = Buf.size(); old_size < new_size; old_size++)
		if (Buf[old_size] == '\n')
			LineOffsets.push_back(old_size);
	ScrollToBottom = true;
}

void AppLog::Draw(const char* title, bool* p_opened)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
	ImGui::Begin(title, p_opened);
	if (ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	if (copy) ImGui::LogToClipboard();

	if (Filter.IsActive())
	{
		const char* buf_begin = Buf.begin();
		const char* line = buf_begin;
		for (int line_no = 0; line != NULL; line_no++)
		{
			const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
			if (Filter.PassFilter(line, line_end))
				ImGui::TextUnformatted(line, line_end);
			line = line_end && line_end[1] ? line_end + 1 : NULL;
		}
	}
	else
	{
		ImGui::TextUnformatted(Buf.begin());
	}

	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::End();
}