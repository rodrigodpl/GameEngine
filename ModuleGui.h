#pragma once

#include "Module.h"
#include "Globals.h"

class ModuleGui : public Module
{
public:
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	bool showdemo;
	bool show_menu;
};
