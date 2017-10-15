#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGui.h"

#include <string>

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	App->gui->app_log.AddLog("Init SDL input event system\n");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->gui->app_log.AddLog("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	dt = (float)ms_timer.Read() / 1000.0f;
	App->gui->Fps_input_data(dt);
	ms_timer.Start();

	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		App->gui->HandleEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
			break;

			case SDL_DROPFILE:     
				

				std::string dropped_file_str(e.drop.file);
				dropped_file_str = dropped_file_str.substr(dropped_file_str.find_last_of(".") + 1);

				if (dropped_file_str == "fbx" || dropped_file_str == "FBX")
					App->importer->LoadFBX(e.drop.file);
				else if (dropped_file_str == "png"|| dropped_file_str == "PNG")
					App->importer->LoadImg(e.drop.file);
				else
					App->gui->app_log.AddLog("Unexpected extension in dropped file!\n");
				
				SDL_free(e.drop.file);
				break;
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	App->gui->app_log.AddLog("Quitting SDL input event subsystem\n");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

