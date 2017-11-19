#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "ModuleFS.h"
#include "Application.h"
#include "Globals.h"

#include <string>

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

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleResources::~ModuleResources()
{
	resources.clear();
}

std::string ModuleResources::ImportFile(const char * new_file_in_assets, bool force)
{
	bool import_ok = false; std::string uid;
	//switch (type) {
	//case Resource::texture: import_ok = App->tex->Import(new_file_in_assets, "", written_file); break;
	//case Resource::audio: import_ok = App->audio->Import(new_file_in_assets, written_file); break;
	//case Resource::scene: import_ok = App->scene->Import(new_file_in_assets, written_file); break;
	//}
	//if (import_ok == true) { // If export was successful, create a new resource
	//	Resource* res = CreateNewResource(type);
	//	res->file = new_file_in_assets;
	//	res->exported_file = written_file;
	//	ret = res->uid;
	//}
	return uid;
}

Resource * ModuleResources::CreateNewResource(Res_Type type, std::string& name)
{
	Resource* ret = nullptr;
	std::string uid;// = CreateUID(name.c_str());
	switch (type) {/*
	case Res_Type::texture: ret = (Resource*) new ResourceTexture(uid); break;
	case Res_Type::mesh: ret = (Resource*) new ResourceMesh(uid); break;
	case Res_Type::audio: ret = (Resource*) new ResourceAudio(uid); break;
	case Res_Type::scene: ret = (Resource*) new ResourceScene(uid); break;
	case Res_Type::bone: ret = (Resource*) new ResourceBone(uid); break;
	case Res_Type::animation: ret = (Resource*) new ResourceAnimation(uid); break;*/
	}
	if (ret != nullptr)
		resources[uid] = ret;
	return ret;
}

Resource* ModuleResources::Get(std::string& uid)
{
	std::map<std::string, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

const Resource* ModuleResources::Get(std::string& uid) const
{
	std::map<std::string, Resource*>::const_iterator it = resources.find(uid);
	if (it != resources.end())
		return it->second;
	return nullptr;
}


Resource* ModuleResources::LoadResource(std::string& imported_file)
{
	Resource* res = nullptr;

	if (res = Get(imported_file))
	{
		uint instances = res->AddInstance();

		if (instances == 1)  // first instance of the object
			res->LoadToMemory();

		return res;
	}
	return nullptr;
}

void ModuleResources::UnLoadResource(std::string& imported_file)
{
	Resource* res = nullptr;

	if (res = Get(imported_file))
	{
		uint instances = res->RemoveInstance();

		if (instances == 0)  // last instance of the object
			res->UnLoadToMemory();
	}
}

