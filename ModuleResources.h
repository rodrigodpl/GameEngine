#pragma once
#include "Module.h"
#include "Resource.h"
#include "ResourceTexture.h"

#include <map>

class ModuleResources : public Module
{
public:            
	    
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	std::string Find(const char* file_in_assets) const;
	std::string ImportFile(const char* new_file_in_assets, bool force = false);
	const Resource* Get(std::string& uid) const;
	Resource* Get(std::string& uid);
	Resource* CreateNewResource(Res_Type type, std::string& name);   

	Resource*	LoadResource(std::string& imported_file);
	void		UnLoadResource(std::string& imported_file);

public:
	std::map<std::string, Resource*> resources;
};
