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

	// config saved variables
	std::string main_cam_obj_uid;

public:
	ModuleEditorCam(Application* app, bool start_enabled = true);
	~ModuleEditorCam();

	bool Start();
	update_status Update(float dt);

	void Save(JSON_file& config);
	void Load(JSON_file& config);

	GameObject* MouseRaycast(Ray ray);

};
