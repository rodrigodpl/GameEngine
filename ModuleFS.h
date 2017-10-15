#pragma once
#include "Module.h"

#include <iostream>
#include <fstream> 

class ModuleFS : public Module
{
public:
	ModuleFS(Application* app, bool start_enabled = true);
	~ModuleFS();

	bool Start();
	bool CleanUp();

	void SaveData();
	void LoadData();

	std::filebuf fb;
};