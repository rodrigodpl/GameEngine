
#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleSceneIntro.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleImporter::~ModuleImporter() {}

bool ModuleImporter::Start() {

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

bool ModuleImporter::CleanUp() {
	
	aiDetachAllLogStreams();

	return true;  
}


GameObject* ModuleImporter::LoadFBX(const char* full_path) {

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	GameObject* root_obj = nullptr;

	if (scene != nullptr) {

		aiNode* root_node = scene->mRootNode;

		for (int i = 0; i < scene->mNumMaterials; i++)
			App->scene_intro->materials.push_back(new ComponentMaterial(*scene->mMaterials[i]));
		
		root_obj = LoadNodeRecursive(root_node, scene);

		aiReleaseImport(scene);
		App->gui->app_log.AddLog("Succesfully loaded %s\n", full_path);
		App->gui->app_log.AddLog("Number of materials %d\n", scene->mNumMaterials);		
	}
	else
		App->gui->app_log.AddLog("Error loading scene %s\n", full_path);

	return root_obj;

}

GameObject* ModuleImporter::LoadNodeRecursive(aiNode* node, const aiScene* scene, GameObject* parent) {

	GameObject* game_object = nullptr;

	//name and parent
	if (node->mName.length > 0)
		game_object = new GameObject(node->mName.C_Str(), parent);
	else
		game_object = new GameObject((game_object->name + "_child").c_str(), parent);


	//transformation
	aiVector3D scl, pos;
	aiQuaternion rot;
	node->mTransformation.Decompose(scl, rot, pos);

	ComponentTransform*	transform = new ComponentTransform(pos, scl, rot);
	game_object->components.push_back((Component*)transform);

	//meshes and materials
	App->gui->app_log.AddLog("Number of meshes %d\n", node->mNumMeshes);
	for (int i = 0; i < node->mNumMeshes; i++) {

		aiMesh* imp_mesh = scene->mMeshes[node->mMeshes[i]];

		ComponentMesh* mesh = new ComponentMesh(*imp_mesh);
		mesh->mat = App->scene_intro->materials.at(imp_mesh->mMaterialIndex);

		game_object->components.push_back((Component*)mesh);
		game_object->components.push_back((Component*)mesh->mat);

	}

	//children
	for (int i = 0; i < node->mNumChildren; i++) {

		aiNode* child_node = node->mChildren[i];
		game_object->children.push_back(LoadNodeRecursive(child_node, scene, game_object));
	}

	return game_object;
}

uint ModuleImporter::LoadImg(const char* full_path) 
{
	App->scene_intro->materials.push_back(new ComponentMaterial(full_path));
	return (App->scene_intro->materials.size() - 1);
}

