#pragma once
#include "Resource.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

enum Tex_Format {
	tex_unknown = -1,
	color_index,
	rgb,
	rgba,
	bgr,
	bgra,
	luminance
};

enum Tex_type {
	type_unkown = -1,
	b,                     //byte
	u_byte,
	shrt,  // short
	u_shrt,
	i,  // int
	u_i,  // uint
	f,  // float
	d,  // double
};

struct TexImportOptions
{
	uint Width = 0;
	uint Height = 0;

	uint Depth = 1;
	uint Bpp = 1;

	Tex_Format Format = rgba;
	Tex_type type = u_byte;
};

class ResourceTexture : public Resource
{
public:
	ResourceTexture(std::string uid);
	~ResourceTexture();

	void LoadToMemory(); 
	void UnLoadToMemory();
public:
	TexImportOptions import_options;
	uint gpu_id = 0;
};