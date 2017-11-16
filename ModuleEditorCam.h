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
	GameObject* main_cam_obj = nullptr;

public:
	ModuleEditorCam(Application* app, bool start_enabled = true);
	~ModuleEditorCam();

	bool Start();
	update_status Update(float dt);
	bool CleanUp(JSON_file& config);

	GameObject* MouseRaycast(Ray ray);

};
