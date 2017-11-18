
#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleSceneIntro.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentAABB.h"
#include "GameObject.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleImporter::~ModuleImporter() {}

bool ModuleImporter::Start() 
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	ilutInit();

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
		App->gui->app_log.AddLog("Successfully loaded %s\n", full_path);
		App->gui->app_log.AddLog("Number of materials %d\n", scene->mNumMaterials);		
	}
	else {
		App->gui->app_log.AddLog("Error loading scene %s\n", full_path);
		root_obj = new GameObject("load error");        // on load failure, return empty game object
	}

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

	ComponentTransform*	transform = new ComponentTransform({ rot.x, rot.y, rot.x, rot.w },{ pos.x, pos.y, pos.z },{ scl.x, scl.y, scl.z });
	game_object->components.push_back((Component*)transform);

	//meshes and materials
	App->gui->app_log.AddLog("Number of meshes %d\n", node->mNumMeshes);
	for (uint i = 0; i < node->mNumMeshes; i++) 
	{
		std::string mesh_name("mesh");
		aiMesh* imp_mesh = scene->mMeshes[node->mMeshes[i]];
		
		ComponentMesh* mesh = new ComponentMesh(*imp_mesh);
		ImportMesh(*imp_mesh, mesh_name.append(std::to_string(i).append(".mesh")));
		mesh->mat = App->scene_intro->materials.at(imp_mesh->mMaterialIndex);

		game_object->components.push_back((Component*)mesh);
	}

	ComponentAABB* aabb = new ComponentAABB(*game_object);
	game_object->components.push_back(aabb);

	//children
	for (uint i = 0; i < node->mNumChildren; i++) 
	{
		aiNode* child_node = node->mChildren[i];
		GameObject* child_obj = LoadNodeRecursive(child_node, scene, game_object);
		game_object->children.push_back(child_obj);
		App->scene_intro->all_game_objects.push_back(child_obj);
	}

	return game_object;
}

uint ModuleImporter::LoadImg(const char* full_path) 
{
	App->scene_intro->materials.push_back(new ComponentMaterial(full_path));
	return (App->scene_intro->materials.size() - 1);
}

void ModuleImporter::LoadAssets(std::vector<std::string> files) 
{
	bool ret = false;

	for (std::vector<std::string>::iterator filename = files.begin(); filename != files.end(); filename++)
	{
		std::string extension;
		supported_extensions file_type = UNKNOWN;

		extension.append(&(*filename).at((*filename).find_last_of(".") + 1));

		if (extension == "png" || extension == "PNG")       file_type = PNG;
		else if (extension == "fbx" || extension == "FBX")  file_type = FBX;
		else if (extension == "wav" || extension == "WAV")  file_type = WAV;

		bool ret = false;

		(*filename).insert(0, "/");
		(*filename).insert(0, ASSETS_BASE_PATH);

		switch (file_type) 
		{
		case PNG: ret = ImportTex((*filename).c_str(), std::string("texture"));
		case FBX: App->scene_intro->AddRootObject(LoadFBX((*filename).c_str()));
		case WAV: // LoadWav((*filename).c_str());

		default: App->gui->app_log.AddLog("unsupported extension found: %s\n", extension.c_str());
		}

		if(!ret)
			App->gui->app_log.AddLog("error loading file: %s ! \n", (*filename).c_str());
	}

}

bool ModuleImporter::ImportTex(const char* imported_file_fullpath, std::string& exported_file_name)
{
	ILuint id; uint ret = 0;
	ilGenImages(1, &id);
	ilBindImage(id);

	ilLoadImage(imported_file_fullpath);

	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
			ret = App->fs->Save(exported_file_name.append(".dds").c_str(), data, LIBRARY_TEXTURES_PATH, size);
		delete[] data;
	}

	ilDeleteImages(1, &id);
	return ret;
}

bool ModuleImporter::ImportTex(const void* buffer, uint size, std::string& exported_file_name)
{
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

	if (size > 0) {
		buffer = new ILubyte[size]; // allocate data buffer

		if (ilSaveL(IL_DDS, (void*)buffer, size) > 0) // Save to buffer with the ilSaveIL function
			return App->fs->Save(exported_file_name.append(".dds").c_str(), buffer, LIBRARY_TEXTURES_PATH, size);
	}
	return false;
}

bool ModuleImporter::LoadTex(const char* exported_file, Texture* resource)
{
	ILuint id; uint ret = 0;
	ilGenImages(1, &id);
	ilBindImage(id);

	std::vector<std::string> textures = App->fs->GetFilesInDir(LIBRARY_TEXTURES_PATH);
	char* buffer = nullptr;

	for (std::vector<std::string>::iterator filename = textures.begin(); filename != textures.end(); filename++)
	{
		if (*filename == exported_file)
		{
			App->scene_intro->materials.push_back(new ComponentMaterial(exported_file));  // TODO: use resources
			return true;
		}
	}
	return false;
}

bool ModuleImporter::ImportMesh(aiMesh& imported_mesh, std::string& exported_file_name)
{
	uint ranges[5];

	ranges[0] = imported_mesh.mNumVertices;								// ranges[0] : vertices
	ranges[1] = imported_mesh.mNumFaces;								// ranges[1] : tris

	if (imported_mesh.HasNormals()) ranges[2] = ranges[0];				// ranges[3] : normals
	else ranges[2] = 0;
	if (imported_mesh.HasVertexColors(0)) ranges[3] = ranges[0];        // ranges[3] : colors
	else ranges[3] = 0;
	if (imported_mesh.HasTextureCoords(0)) ranges[4] = ranges[0];       // ranges[4] : texcoords
	else ranges[4] = 0;


	uint size = sizeof(ranges) + sizeof(float3) * ranges[0] + sizeof(Tri) * ranges[1] + sizeof(float3) * ranges[2] + sizeof(Color) * ranges[3] + sizeof(float2) * ranges[4];

	char* data = new char[size];												// Allocate
	char* cursor = data;	
			
	memcpy(cursor, ranges, sizeof(ranges));										// First store ranges

	uint bytes = sizeof(ranges);
	cursor += bytes;															// Store vertices
	memcpy(cursor, imported_mesh.mVertices, sizeof(float3) * ranges[0]);

	bytes = sizeof(float3) * ranges[0];
	cursor += bytes;

	bytes = sizeof(Tri);
	for (uint i = 0; i < ranges[1]; ++i)										// Store tris
	{
		if (imported_mesh.mFaces[i].mNumIndices == 3)
		{
			memcpy(cursor, imported_mesh.mFaces[i].mIndices, sizeof(Tri));
			cursor += bytes;
		}
	}
		 
	memcpy(cursor, imported_mesh.mNormals, sizeof(float3) * ranges[2]);			// Store normals

	bytes = sizeof(float3) * ranges[2];											// Store colors
	cursor += bytes;
	memcpy(cursor, imported_mesh.mColors, sizeof(Color) * ranges[3]);
	
	bytes = sizeof(Color) * ranges[3];											// Store texcoords
	cursor += bytes;
	memcpy(cursor, imported_mesh.mColors, sizeof(float2) * ranges[4]);

	return App->fs->Save(exported_file_name.c_str(), data, LIBRARY_MESHES_PATH, size);
}

bool ModuleImporter::LoadMesh(const char* exported_file, ComponentMesh* resource)
{
	uint size = 0;
	char* buffer = App->fs->Load(exported_file, size);

	if (size) {
		char* cursor = buffer;

		uint ranges[5];					 // amount of vertices / tris / normals / colors / texture_coords
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		resource->num_vertices = ranges[0];
		resource->num_tris = ranges[1];
		resource->num_normals = ranges[2];
		resource->num_colors = ranges[3];
		resource->num_texcoords = ranges[4];

		// Load vertices
		cursor += bytes;
		bytes = sizeof(float3) * resource->num_vertices;
		resource->vertices = new float3[resource->num_vertices];
		memcpy(resource->vertices, cursor, bytes);

		// Load indices
		cursor += bytes;
		bytes = sizeof(Tri) * resource->num_tris;
		resource->tris = new Tri[resource->num_tris];
		memcpy(resource->tris, cursor, bytes);

		// Load normals
		cursor += bytes;
		bytes = sizeof(float3) * resource->num_normals;
		resource->normals = new float3[resource->num_normals];
		memcpy(resource->normals, cursor, bytes);

		// Load colors
		cursor += bytes;
		bytes = sizeof(Color) * resource->num_colors;
		resource->colors = new Color[resource->num_colors];
		memcpy(resource->colors, cursor, bytes);

		// Load texcoords
		cursor += bytes;
		bytes = sizeof(float2) * resource->num_texcoords;
		resource->texcoords = new float2[resource->num_texcoords];
		memcpy(resource->texcoords, cursor, bytes);

	}
	else
		App->gui->app_log.AddLog("error loading exported file: %s", exported_file);

	return size;
}