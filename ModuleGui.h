#pragma once

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "SDL\include\SDL.h"

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


};

