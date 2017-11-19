#pragma once
#include "Resource.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew-2.1.0\include\GL\glew.h"

#include "MathGeoLib.h"
#include "Color.h"

class ResourceTexture;

struct Tri 
{
	uint vert1 = 0;
	uint vert2 = 0;
	uint vert3 = 0;
};


class ResourceMesh : public Resource {

public:
	ResourceMesh(std::string uid);
	~ResourceMesh();

	void LoadToMemory();
	void UnLoadToMemory();

public:
	char* buffer = nullptr;

	uint id_tris = 0;
	uint num_tris = 0;
	Tri* tris = nullptr;   // tris are indices indexed by the triangles they form

	uint id_normals = 0;
	uint num_normals = 0;
	float3* normals = nullptr;

	uint id_colors = 0;
	uint num_colors = 0;
	Color* colors = nullptr;

	uint id_texcoords = 0;
	uint num_texcoords = 0;
	float2* texcoords = nullptr;

	uint num_vertices = 0;
	float3* vertices = nullptr;     // vertices change with object transformation, so its ComponentMesh who handles vertices_id

	std::string bound_tex_path = nullptr;
};