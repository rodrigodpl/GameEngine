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

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		App->gui->app_log.AddLog("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = screen_width * screen_size;
		int height = screen_heigth * screen_size;
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

		if(window == NULL)
		{
			App->gui->app_log.AddLog("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
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
	config.WriteNumber("window.screen_heigth", screen_heigth);
	config.WriteNumber("window.screen_size", screen_size);
}

void ModuleWindow::Load(JSON_file& config)
{
	win_mode = (WindowMode)((int)config.ReadNumber("window.window_mode"));
	screen_width = config.ReadNumber("window.screen_width");
	screen_heigth = config.ReadNumber("window.screen_heigth");
	screen_size = config.ReadNumber("window.screen_size");
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}