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

	const Resource* Get(std::string& uid) const;
	Resource* Get(std::string& uid);

	Resource*	LoadResource(std::string& imported_file);
	void		UnLoadResource(std::string& imported_file);

public:
	std::map<std::string, Resource*> resources;
};
