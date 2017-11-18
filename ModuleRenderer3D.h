#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8


class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Save(JSON_file& config);
	void Load(JSON_file& config);

	void DebugDraw();
	void OnResize(int width, int height);

public:

	bool debug_draw = false;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	// config saved variables
	bool	vsync = true;

private:
	Timer	ms_timer;
	float	dt;

	// config saved variables
	bool	face_culling = true;
	Color	clear_color =	{ 0.9f, 0.9f, 0.9f, 1.0f };
	Color	light_ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	Color	main_light =	{ 0.7f, 0.7f, 0.7f, 1.0f };
};