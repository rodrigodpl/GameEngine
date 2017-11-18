#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	App->gui->app_log.AddLog("Init SDL window & surface\n");
	bool ret = true;

	config = App->json->OpenFile("config.json", SETTINGS_BASE_PATH);

	if (config->ReadNumber("window.screen_size") >= 0) {
		win_size = config->ReadNumber("window.screen_size");
	}
	else win_size = 1;

	if (config->ReadNumber("window.window_mode") >= 0) window_option = config->ReadNumber("window.window_mode");
	else window_option = 1;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		App->gui->app_log.AddLog("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		ret = CreateNewWindow();
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	App->gui->app_log.AddLog("Destroying SDL window and quitting all SDL systems\n");

	if(window) SDL_DestroyWindow(window);
	SDL_Quit();

	return true;
}

void ModuleWindow::Save(JSON_file& config)
{
	config.WriteNumber("window.window_mode", win_mode);
	config.WriteNumber("window.screen_width", screen_width);
	config.WriteNumber("window.screen_heigth", screen_height);
	config.WriteNumber("window.screen_size", screen_size);
}

void ModuleWindow::Load(JSON_file& config)
{
	win_mode = (WindowMode)((int)config.ReadNumber("window.window_mode"));
	screen_width = config.ReadNumber("window.screen_width");
	screen_height = config.ReadNumber("window.screen_heigth");
	screen_size = config.ReadNumber("window.screen_size");
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetWindowsSize(int value) {
	if (value != win_size) {
		switch (value) {
		case 0:
			screen_width = 640;
			screen_height = 480;
			break;
		case 1:
			screen_width = 800;
			screen_height = 600;
			break;
		case 2:
			screen_width = 960;
			screen_height = 720;
			break;
		case 3:
			screen_width = 1024;
			screen_height = 576;
			break;
		case 4:
			screen_width = 1024;
			screen_height = 768;
			break;
		case 5:
			screen_width = 1152;
			screen_height = 648;
			break;
		case 6:
			screen_width = 1280;
			screen_height = 720;
			break;
		case 7:
			screen_width = 1280;
			screen_height = 800;
			break;
		case 8:
			screen_width = 1280;
			screen_height = 960;
			break;
		case 9:
			screen_width = 1366;
			screen_height = 768;
			break;
		case 10:
			screen_width = 1440;
			screen_height = 900;
			break;
		case 11:
			screen_width = 1400;
			screen_height = 1050;
			break;
		case 12:
			screen_width = 1440;
			screen_height = 1080;
			break;
		case 13:
			screen_width = 1600;
			screen_height = 900;
			break;
		case 14:
			screen_width = 1600;
			screen_height = 1200;
			break;
		case 15:
			screen_width = 1680;
			screen_height = 1050;
			break;
		case 16:
			screen_width = 1856;
			screen_height = 1392;
			break;
		case 17:
			screen_width = 1920;
			screen_height = 1440;
			break;
		case 18:
			screen_width = 1920;
			screen_height = 1080;
			break;
		case 19:
			screen_width = 1920;
			screen_height = 1200;
			break;
		case 20:
			screen_width = 2048;
			screen_height = 1536;
			break;
		case 21:
			screen_width = 2560;
			screen_height = 1440;
			break;
		case 22:
			screen_width = 2560;
			screen_height = 1600;
			break;
		case 23:
			screen_width = 3840;
			screen_height = 2160;
			break;
		default:
			App->gui->app_log.AddLog("Could not determine requested windows size, setting it into default\n");
			screen_width = 800;
			screen_height = 600;
		}
		win_size = value;
		CreateNewWindow();
	}
}

bool ModuleWindow::CreateNewWindow() {
	//Create window
	SDL_DestroyWindow(window);

	int width = screen_width * screen_size;
	int height = screen_height * screen_size;
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	switch (win_mode)
	{
	case FULLSCREEN: flags |= SDL_WINDOW_FULLSCREEN; break;
	case RESIZABLE: flags |= SDL_WINDOW_RESIZABLE; break;
	case BORDERLESS: flags |= SDL_WINDOW_BORDERLESS; break;
	case FULLSCREEN_DESKTOP: flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; break;
	}

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

	if (window == NULL)
	{
		App->gui->app_log.AddLog("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		//Get window surface
		screen_surface = SDL_GetWindowSurface(window);
		return true;
	}
}

void ModuleWindow::SetWindowMode(int mode) {
	if (win_mode != mode) {
		win_mode = (WindowMode)mode;
		CreateNewWindow();
	}
}