#pragma once
#include "Module.h"
#include "Globals.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "glmath.h"

class ModuleEditorCam : public Module
{
public:
	ComponentCamera* cam = nullptr;

public:
	ModuleEditorCam(Application* app, bool start_enabled = true);
	~ModuleEditorCam();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

};