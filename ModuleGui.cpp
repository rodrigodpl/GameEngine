#include "ModuleGui.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "Globals.h"
#include "Application.h"

#include <Windows.h>
#include <stdlib.h>

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGui::~ModuleGui(){}

bool ModuleGui::Start() {

	ImGui_ImplSdl_Init(App->window->window);
	/*ImGuiStyle * style = &ImGui::GetStyle();
	style->Alpha = 1.0f;*/

	GetSystemInfo(&SysInfo);

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
			if (ImGui::MenuItem("ShowAbout")) 
				show_about = !show_about;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Hardware"))
		{
			if (ImGui::MenuItem("ShowHardware"))
				show_hardware = !show_hardware;

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	// ------------------


	// Test window ------
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		showdemo = !showdemo;


	// Log Window -----
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		show_log = !show_log;


	//Blits
	if (showdemo)
		ImGui::ShowTestWindow(&showdemo);

	if (show_log)
		app_log.Draw("Log", &show_log);

	if(show_about)
		DrawAbout();

	if(show_hardware)
		DrawHardware();
	// ----------

	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate(float dt) {
	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleGui::CleanUp()
{
	App->gui->app_log.AddLog("Unloading Intro scene\n");
	return true;
}

void ModuleGui::showaboutmenu() {
	
}

void ModuleGui::DrawHardware() {

	ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Hardware", &show_hardware, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
	ImGui::Text("Number of processors");
	ImGui::Text("%d", SysInfo.dwNumberOfProcessors);
	ImGui::Separator();
	ImGui::Text("Processor Architecture");
	ImGui::Text("%d", SysInfo.wProcessorArchitecture);
	ImGui::Separator();
	ImGui::End();
}

void ModuleGui::DrawAbout() {

	ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("About", &show_about, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
	ImGui::Text("Space Engine");
	ImGui::Text("It renders in parseks");
	ImGui::Separator();
	ImGui::Text("By Rodrigo de Pedro & Daniel Garcia");
	ImGui::End();
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




