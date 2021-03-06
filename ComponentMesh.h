#pragma once
#include "Globals.h"
#include "Component.h"
#include "ComponentMaterial.h"
#include "ResourceMesh.h"

#include "MathGeoLib.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew-2.1.0\include\GL\glew.h"



class ComponentTransform;

class ComponentMesh : Component {
public:

	ComponentMesh(aiMesh& mesh);
	ComponentMesh(ComponentMesh& mesh);
	ComponentMesh();

	~ComponentMesh();

	void Save(JSON_file& save_file, const char* component_code);
	void Load(JSON_file& save_file, const char* component_code);

	bool LoadDataFromAssimp(aiMesh& mesh);
	void LoadDataToVRAM();

	void LoadFromBuffer(char* buffer);

	virtual void Draw();

	Component* Duplicate();

public:
	uint id_tris = 0;
	uint num_tris = 0;
	Tri* tris = nullptr;

	uint id_vertices = 0;
	uint num_vertices = 0;
	float3* vertices = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;
	float3* normals = nullptr;

	uint id_colors = 0;
	uint num_colors = 0;
	Color* colors = nullptr;

	uint id_texcoords = 0;
	uint num_texcoords = 0;
	float2* texcoords = nullptr;

	ComponentMaterial* mat = nullptr;
	ComponentTransform* transform = nullptr;			// scene saved variable
	std::string imported_file;
	uint imported_f_length = 0;
};
