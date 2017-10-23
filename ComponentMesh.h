#pragma once
#include "Globals.h"
#include "Component.h"
#include "ComponentMaterial.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew-2.1.0\include\GL\glew.h"

class ComponentMesh : Component {
public:

	ComponentMesh();
	ComponentMesh(aiMesh& mesh);
	ComponentMesh(ComponentMesh& mesh);

	~ComponentMesh();

	bool LoadDataFromAssimp(aiMesh& mesh);
	void LoadDataToVRAM();

	virtual void Draw();

	Component* Duplicate();

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

	ComponentMaterial* mat = nullptr;
};
