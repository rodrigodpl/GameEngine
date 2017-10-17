#pragma once
#include "Globals.h"
#include "Component.h"

#include "glew-2.1.0\include\GL\glew.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

class ComponentMesh : Component {
public:

	ComponentMesh();
	ComponentMesh(aiMesh& mesh, char* tex_path = "");
	ComponentMesh(ComponentMesh& mesh);

	~ComponentMesh();

	bool LoadDataFromAssimp(aiMesh& mesh);
	void LoadDataToVRAM();
	void LoadTex(char* tex_path);
	void CheckeredTexture();

	void Draw();

	Component* Duplicate();
	void Update();

public:
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
	GLuint bound_tex = 0;
};
