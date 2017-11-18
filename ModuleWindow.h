#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

enum WindowMode {

	FULLSCREEN,
	RESIZABLE,
	BORDERLESS,
	FULLSCREEN_DESKTOP 
};

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void Save(JSON_file& config);
	void Load(JSON_file& config);

	void SetTitle(const char* title);
	void SetWindowsSize(int value);

public:
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;

	// config saved variables
	WindowMode win_mode = RESIZABLE;
	int screen_width = 800;
	int screen_height = 600;
	float screen_size = 1.0f;

private:
	bool CreateNewWindow();
	int win_size;
	int window_option;

	JSON_file* config = nullptr;
};

#endif // __ModuleWindow_H__