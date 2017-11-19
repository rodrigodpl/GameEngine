#pragma once

#include "Module.h"
#include "ResourceTexture.h"

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
	bool ImportTex(const char* imported_file_fullpath);
	void SaveTexMetaFile(JSON_file& meta_file, TexImportOptions& options);
	TexImportOptions GetTexImportOptionsFromBoundImg();				// must be called only when there's a DevIL bound image
	//bool LoadCheckers(Texture* resource);

	// Meshes: 
	bool ImportMesh(aiMesh& imported_mesh, std::string& exported_file_name);
	//bool SaveMeshMetaFile(JSON_file& meta_file, TexImportOptions& options);

};