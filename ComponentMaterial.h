#pragma once
#include "Globals.h"

#include "Component.h"

#include <string>
#include <vector>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

struct Texture 
{
	aiString path;
	uint width = 0, height = 0;
	uint gl_binding = 0;
};

class ComponentMaterial : Component {
public:

	ComponentMaterial();
	ComponentMaterial(const char* tex_full_path);
	ComponentMaterial(aiMaterial& mat);
	ComponentMaterial(ComponentMaterial& transform);

	~ComponentMaterial();

	void CheckeredTexture();

	Component* Duplicate();
	void Update() {};

public:

	std::vector<Texture*> textures;
};
