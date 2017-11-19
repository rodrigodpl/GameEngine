
#include "Application.h"
#include "Brofiler.h"


Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleEditorCam(this);
	gui = new ModuleGui(this);
	importer = new ModuleImporter(this);
	json = new ModuleJSON(this);
	fs = new ModuleFS(this);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(audio);
	AddModule(importer);
	AddModule(json);
	AddModule(gui);
	AddModule(fs);
	AddModule(camera);         

	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;
	dtmod = 1.0f;
	// Call Init() in all modules
	config = json->OpenFile("config.json", SETTINGS_BASE_PATH);
	p2List_item<Module*>* item = list_modules.getFirst();

	if (!config) return false;                                 // early exit if config cannot be loaded nor created
	else LoadModules();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	gui->app_log.AddLog("Application Start --------------\n");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
	ms_timer.Start();
	return ret;
}

void Application::PrepareUpdate()
{
	dt = ((float)ms_timer.Read() / 1000.0f) / dtmod;
	gui->Fps_app_data(dt);
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{BROFILER_FRAME("Game")
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	SaveModules();
	config->Save();
	config->CleanUp();

	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

float Application::Getdt() {
	return dtmod;
}

void Application::Changedt(float newdt) {
	dtmod = newdt;
}

void Application::SaveModules()
{
	config->WriteBool("empty", false);

	p2List_item<Module*>* item = list_modules.getLast();

	while (item)
	{
		item->data->Save(*config);
		item = item->prev;
	}

}

void Application::LoadModules()
{
	bool empty_config = config->ReadBool("empty");

	if (!empty_config) {

		p2List_item<Module*>* item = list_modules.getLast();

		while (item)
		{
			item->data->Load(*config);
			item = item->prev;
		}
	}
	else
		gui->app_log.AddLog("empty config file! the engine will start with default values\n");
}
