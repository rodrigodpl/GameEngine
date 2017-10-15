#include "ModuleFS.h"
#include "Application.h"

#include "cereal\cereal.hpp"
#include "cereal\archives\json.hpp"

ModuleFS::ModuleFS(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleFS::~ModuleFS() {}

bool ModuleFS::Start() {

	fb.open("test.json", std::ios::out);
	std::ostream os(&fb);

	cereal::JSONOutputArchive archive(os);

	int a = 5;
	archive(a);

	return true;
}

bool ModuleFS::CleanUp() 
{
	fb.close();

	return true;
}