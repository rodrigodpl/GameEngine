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
class ResourceMesh;

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
	void ImportAssets(std::vector<std::string> files);
	
	// FBX:											not a resource itself, but is decomposed into meshes and textures
	void ImportFBX(const char* full_path);
	void ImportNodeRecursive(aiNode* node, const aiScene* scene);

	// Textures:
	ResourceTexture* ImportTex(const char* imported_file_fullpath);
	void SaveTexMetaFile(JSON_file& meta_file, TexImportOptions& options);
	TexImportOptions GetTexImportOptionsFromBoundImg();				// must be called only when there's a DevIL bound image
	//bool LoadCheckers(Texture* resource);

	// Meshes: 
	uint ImportMesh(aiMesh& imported_mesh, std::string& exported_file_name);
	//bool SaveMeshMetaFile(JSON_file& meta_file, TexImportOptions& options);

};