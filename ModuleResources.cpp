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

