#pragma once

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "SDL\include\SDL.h"
#include "Color.h"

#include <vector>
#include <string>

#define GRAPH_SIZE 100
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

struct AppLog
{
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset
	bool                ScrollToBottom;

	void    Clear();
	void    AddLog(const char* fmt, ...) IM_PRINTFARGS(2);
	void    Draw(const char* title, bool* p_opened = NULL);
};

class ModuleGui : public Module
{
public:
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool Start();
	update_status PreUpdate(float dt);
	void HandleEvent(SDL_Event* sdl_event);
	bool CleanUp();
	void Draw();

	AppLog app_log;

private:
	bool draw_demo = false;
	bool draw_menu = true;
	bool draw_about = false;
	bool draw_log = false;
	bool draw_hardware = false;
	bool draw_performance = false;
	bool draw_options = false;
	bool draw_properties = false;

	float avg;
	int prev;
	std::string fps_text;
	std::string n_indices;
	std::string n_vertices;

	std::vector<float> fps_app;
	std::vector<float> fps_renderer;
	std::vector<float> fps_physics;
	std::vector<float> fps_input;

	void Calc_avg(std::vector<float> vector);

	float* rgb_data;

public:
	void Fps_app_data(float aux);
	void Fps_renderer_data(float aux);
	void Fps_physics_data(float aux);
	void Fps_input_data(float aux);

	void Get_colour(Color aux);
};

