#include "ModuleGui.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "ImGuizmo.h"
#include "Globals.h"
#include "GameObject.h"
#include "Application.h"

#include <stdlib.h>

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGui::~ModuleGui() {}

bool ModuleGui::Start() {

	ImGui_ImplSdl_Init(App->window->window);
	fps_app.resize(GRAPH_SIZE);
	fps_renderer.resize(GRAPH_SIZE);
	fps_input.resize(GRAPH_SIZE);
	fps_physics.resize(GRAPH_SIZE);

	prev = 0;
	rgb_data = new float[3];

	return true;
}

update_status ModuleGui::PreUpdate(float dt) {
	//  Main Menu -------
	ImGui_ImplSdl_NewFrame(App->window->window);
	ImGuizmo::BeginFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File", &draw_menu))
		{
			if (ImGui::MenuItem("Save")) 
			{
				App->SaveModules();
			}
			if (ImGui::MenuItem("Load"))
			{
				App->LoadModules();
			}
			if (ImGui::MenuItem("Options"))
			{
				draw_options = !draw_options;
			}
			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Hardware"))
				draw_hardware = !draw_hardware;
			if (ImGui::MenuItem("Properties"))
				draw_properties = !draw_properties;
			if (ImGui::MenuItem("Console"))
				draw_log = !draw_log;
			if (ImGui::MenuItem("Performance"))
				draw_performance = !draw_performance;
			if (ImGui::MenuItem("Transform"))
				draw_transform = !draw_transform;
			if (ImGui::MenuItem("Hierarchy"))
				draw_hierarchy = !draw_hierarchy;
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
	App->gui->app_log.AddLog("Unloading Gui\n");
	ImGui_ImplSdl_Shutdown();
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
		ImGui::Text("Assimp        v.3.1.1");
		ImGui::Text("DevIL         v.1.7.8");
		ImGui::Separator();
		if (ImGui::Button("Github")) ShellExecute(0, 0, "https://github.com/rodrigodpl/GameEngine", 0, 0, SW_SHOW);
		ImGui::End();
	}

	if (draw_performance) {
		ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Performance", &draw_performance, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
		ImGui::Text("Application");
		Calc_avg(fps_app);
		fps_text = std::to_string(avg) + " FPS";
		char *cptr = new char[fps_text.size()];
		std::strncpy(cptr, fps_text.c_str(), fps_text.size());
		ImGui::PlotHistogram("", &fps_app[0], fps_app.size(), 0, cptr, 0.0f, 100.0f, ImVec2(200,100));
		ImGui::Separator();
		delete[] cptr;
		// -------------------  
		if (ImGui::CollapsingHeader("Renderer")) {
			Calc_avg(fps_renderer);
			fps_text = std::to_string(avg) + " MS";
			char *crend = new char[fps_text.size()];
			std::strncpy(cptr, fps_text.c_str(), fps_text.size());
			ImGui::PlotHistogram("", &fps_renderer[0], fps_renderer.size(), 0, cptr, 0.0f, 0.05f, ImVec2(200, 100));
			delete[] crend;
		}
		if (ImGui::CollapsingHeader("Physics")) {
			Calc_avg(fps_physics);
			fps_text = std::to_string(avg) + " MS";
			char *cphys = new char[fps_text.size()];
			std::strncpy(cptr, fps_text.c_str(), fps_text.size());
			ImGui::PlotHistogram("", &fps_physics[0], fps_physics.size(), 0, cptr, 0.0f, 0.05f, ImVec2(200, 100));
			delete[] cphys;
		}
		if (ImGui::CollapsingHeader("Input")) {
			Calc_avg(fps_input);
			fps_text = std::to_string(avg) + " MS";
			char *cinp = new char[fps_text.size()];
			std::strncpy(cptr, fps_text.c_str(), fps_text.size());
			ImGui::PlotHistogram("", &fps_input[0], fps_input.size(), 0, cptr, 0.0f, 0.05f, ImVec2(200, 100));
			delete[] cinp;
		}
		ImGui::End();
	}

	if (draw_options)
	{
		ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Options", &draw_options, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
		if (ImGui::CollapsingHeader("Display")) {
			static int win_size = 1;
			const char* sizes[] = { "640x480", "800x600", "960x720", "1024x576", "1024x768", "1152x648", "1280x720", "1280x800", "1280x960", "1366x768", "1440x900", "1400x1050", "1440x1080", "1600x900", "1600x1200", "1680x1050", "1856x1392", "1920x1440", "1920x1080", "1920x1200", "2048x1536", "2560x1440", "2560x1600", "3840x2160" };
			
			ImGui::Text("Window Size"); ImGui::SameLine();
			ImGui::Combo("", &win_size, sizes, IM_ARRAYSIZE(sizes));
			ImGui::Separator();
			static bool vsync = true;
			ImGui::Checkbox("Vsync", &vsync);
			//App->json->config->WriteBool("Vsync", vsync);
			ImGui::Separator();
			static int e = 1;
			ImGui::RadioButton("Fullscreen", &e, 0);
			ImGui::RadioButton("Windowed", &e, 1); 
			ImGui::RadioButton("Borderless", &e, 2);
		}
		if (ImGui::CollapsingHeader("Input")) {
			
		}
		if (ImGui::CollapsingHeader("Renderer")) {
			static bool wireframe = false;
			ImGui::Checkbox("Wireframe", &wireframe);
			ImGui::Text("Object colour");
			ImGui::ColorPicker3("", rgb_data);
			ImGui::Separator();
			static int draw_stuff = 0;
			const char* prim[] = { "", "Cube", "Cylinder", "Sphere" };
			ImGui::Combo("", &draw_stuff, prim, IM_ARRAYSIZE(prim));
			if (draw_stuff != 0 && prev != draw_stuff) {
				/*switch (draw_stuff) {
				case 1:
					App->scene_intro->AddCube(1, 1, 1);
					break;
				case 2:
					App->scene_intro->AddCylinder(1, 1, 30);
					break;
				case 3:
					App->scene_intro->AddSphere(1, 16, 16);
				}*/
				prev = draw_stuff;
			}
		}
		if (ImGui::CollapsingHeader("Textures")) {
			static int shader = 0;
			ImGui::Text("Shader"); ImGui::SameLine();
			ImGui::Combo("", &shader, "Basic\0Checker\0\0");
		}
		ImGui::End();
	}

	if (draw_properties) {
		ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Properties", &draw_properties, ImGuiWindowFlags_ShowBorders);
		if (ImGui::CollapsingHeader("Transformation")) {
			//Position
			if (ImGui::CollapsingHeader("Position")) {
				//App->scene_intro->selected_game_obj
				/*ImGui::InputInt("", &transX); ImGui::SameLine();
				ImGui::InputInt("", &transY); ImGui::SameLine();
				ImGui::InputInt("", &transZ);*/
			}
			//Rotation
			if (ImGui::CollapsingHeader("Rotation")) {
				/*ImGui::SliderFloat("", &rotX, -180.0f, 180.0f, "%.0f"); ImGui::SameLine();
				ImGui::SliderFloat("", &rotY, -180.0f, 180.0f, "%.0f"); ImGui::SameLine();
				ImGui::SliderFloat("", &rotZ, -180.0f, 180.0f, "%.0f");*/
			}
			//Scale
			if (ImGui::CollapsingHeader("Scale")) {
				/*ImGui::InputInt("", &scaleX); ImGui::SameLine();
				ImGui::InputInt("", &scaleY); ImGui::SameLine();
				ImGui::InputInt("", &scaleZ);*/
			}
		}
		if (ImGui::CollapsingHeader("Geometry")) {
			//ImGui::Text("# Indices: "); ImGui::SameLine();
			//ImGui::Text(App->scene_intro->meshes.back().num_indices);
			//ImGui::Text("# Vertices: " + App->scene_intro->meshes.back().num_vertices);
		}
		if (ImGui::CollapsingHeader("Texture")) {

		}
		ImGui::End();
	}

	if (draw_play) {
		float dt = App->Getdt();
		ImGui::SetNextWindowPos(ImVec2(277, 19), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Play", &draw_play, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::Button(">", ImVec2(50, 25))) {
			dt = 1.0f;
		}
		ImGui::SameLine();
		if (ImGui::Button("||", ImVec2(50, 25))) {
			dt = 0.0f;
		}
		/*ImGui::SameLine();
		if (ImGui::Button(">|", ImVec2(50, 25))) {

		}*/
		/*ImGui::SameLine();
		if (ImGui::Button("[]", ImVec2(50,25))) {
		
		}*/
		ImGui::SliderFloat("", &dt, 0.0f, 1.0f, "%.2f");
		App->Changedt(dt);
		ImGui::End();
	}

	if (draw_hierarchy) {
		ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Hierarchy", &draw_transform, ImGuiWindowFlags_ShowBorders);
		for (std::vector<GameObject*>::iterator it = App->scene_intro->game_objects.begin(); it != App->scene_intro->game_objects.end(); it++) {
			(*it)->CreateTree();
		}

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

void ModuleGui::Calc_avg(std::vector<float> aux) {
	float num_values = 0;
	avg = 0;

	for (int counter = 0; counter < aux.size(); counter++) {
		avg += aux[counter];
		num_values++;
	}

	avg = avg / num_values;
}

// All the data for each graph

void ModuleGui::Fps_app_data(float aux)
{
	if (fps_app.capacity() != fps_app.size())
	{
		fps_app.push_back(1 / aux);
	}
	else
	{
		for (int val = 0; val < (GRAPH_SIZE - 1); val++)
		{
			fps_app[val] = fps_app[val + 1];
		}
		fps_app[fps_app.size() - 1] = 1 / aux;
	}
}

void ModuleGui::Fps_renderer_data(float aux)
{
	if (fps_renderer.capacity() != fps_renderer.size())
	{
		fps_renderer.push_back(aux);
	}
	else
	{
		for (int val = 0; val < (GRAPH_SIZE - 1); val++)
		{
			fps_renderer[val] = fps_renderer[val + 1];
		}
		fps_renderer[fps_renderer.size() - 1] = aux;
	}
}

void ModuleGui::Fps_input_data(float aux)
{
	if (fps_input.capacity() != fps_input.size())
	{
		fps_input.push_back(aux);
	}
	else
	{
		for (int val = 0; val < (GRAPH_SIZE - 1); val++)
		{
			fps_input[val] = fps_input[val + 1];
		}
		fps_input[fps_input.size() - 1] = aux;
	}
}

void ModuleGui::Fps_physics_data(float aux)
{
	if (fps_physics.capacity() != fps_physics.size())
	{
		fps_physics.push_back(aux);
	}
	else
	{
		for (int val = 0; val < (GRAPH_SIZE - 1); val++)
		{
			fps_physics[val] = fps_physics[val + 1];
		}
		fps_physics[fps_physics.size() - 1] = aux;
	}
}

void ModuleGui::Get_colour(Color aux) {
	aux.Set(rgb_data[0], rgb_data[1], rgb_data[2], 1.0f);
}

void ModuleGui::CreateTree() {
	//App->scene_intro->game_objects
	//ImGui::TreeNode()
}