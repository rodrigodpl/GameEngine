#pragma once

#include "Module.h"

#include <string>
#include <vector>

class Texture;
class aiMesh;
class aiNode;
class aiScene;
class ComponentMesh;
class GameObject;

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
	bool CleanUp();

	GameObject* LoadFBX(const char* full_path);
	GameObject* LoadNodeRecursive(aiNode* node, const aiScene* scene, GameObject* parent = nullptr);

	uint LoadImg(const char* full_path);

	void LoadAssets(std::vector<std::string> files);

	// Textures:
	bool ImportTex(const char* imported_file_fullpath, std::string& exported_file_name);
	bool ImportTex(const void* buffer, uint size, std::string& exported_file_name);
	bool LoadTex(const char* exported_file, Texture* resource);
	//bool LoadCheckers(Texture* resource);

	// Meshes: 
	bool ImportMesh(aiMesh& imported_mesh, std::string& exported_file_name);
	bool LoadMesh(const char* exported_file, ComponentMesh* resource);

};