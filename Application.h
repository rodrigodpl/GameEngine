#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditorCam.h"
#include "ModuleGui.h"
#include "ModuleImporter.h"
#include "ModuleResources.h"
#include "ModuleJSON.h"
#include "ModuleFS.h"

class JSON_file;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleResources* resources;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleEditorCam* camera;
	ModuleGui* gui;
	ModuleImporter* importer;
	ModuleJSON* json;
	ModuleFS* fs;

private:

	Timer	ms_timer;
	float	dt;
	float	dtmod;
	p2List<Module*> list_modules;
	JSON_file* config = nullptr;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void SaveModules();
	void LoadModules();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	float Getdt();
	void Changedt(float newdt);
};