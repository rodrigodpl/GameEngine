#pragma once

#include "Module.h"
#include "GameObject.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

enum supported_extensions
{
	UNKNOWN,
	PNG,
	FBX,
	WAV
};


class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	bool CleanUp(JSON_file& config);

	GameObject* LoadFBX(const char* full_path);
	GameObject* LoadNodeRecursive(aiNode* node, const aiScene* scene, GameObject* parent = nullptr);

	uint LoadImg(const char* full_path);

	void LoadAssets(std::vector<std::string> files);

};