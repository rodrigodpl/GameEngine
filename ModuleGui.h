#pragma once

#include "Module.h"
#include "Globals.h"
#include "imgui.h"

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
	update_status PostUpdate(float dt);
	void DrawHardware();
	void DrawAbout();
	bool CleanUp();

	AppLog app_log;
	SYSTEM_INFO SysInfo;

private:
	bool showdemo = false;
	bool show_menu = true;
	bool show_about = false;
	bool show_log = false;
	bool show_hardware = true;

	void showaboutmenu();
};

