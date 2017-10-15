#pragma once

#include "Module.h"
#include "glew-2.1.0\include\GL\glew.h"

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

struct Mesh {

	uint id_vertices = 0; 
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0; 
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_colors = 0;
	uint num_colors = 0;
	float* colors = nullptr;

	uint id_texcoords = 0;
	uint num_texcoords = 0;
	float* texcoords = nullptr;

	bool visible = true;

};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	bool CleanUp();
	void LoadFBX(const char* full_path);
	void LoadTex(const char* full_path);

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	uint image_id;
};