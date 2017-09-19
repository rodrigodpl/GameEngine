#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleGui : public Module
{
public:
	ModuleGui();
	~ModuleGui();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
};
