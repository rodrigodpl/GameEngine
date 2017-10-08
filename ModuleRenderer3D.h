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
	void DebugDraw();

	void OnResize(int width, int height);

public:

	bool debug_draw;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	uint my_id = 0;
	uint my_indices = 0;
	Vertex* vertices = new Vertex[8];
	uint* indices = new uint[36];
};