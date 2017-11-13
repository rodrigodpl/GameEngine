#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"

class GameObject;
class ComponentCamera;


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

	GameObject* MouseRaycast(Ray ray);

};
