#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"


#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled), music(NULL)
{}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init(JSON_file& config)   // audio channels
{
	App->gui->app_log.AddLog("Loading Audio Mixer\n");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		App->gui->app_log.AddLog("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// load support for the OGG format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		App->gui->app_log.AddLog("Could not initialize Mixer lib. Mix_Init: %s\n", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		App->gui->app_log.AddLog("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	App->gui->app_log.AddLog("Freeing sound FX, closing Mixer and Audio subsystem\n");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;

	for(item = fx.getFirst(); item != NULL; item = item->next)
	{
		Mix_FreeChunk(item->data);
	}

	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}

void ModuleAudio::Save(JSON_file& config)
{
	config.WriteBool("audio.stereo", stereo);
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;
	
	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic((int) (fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		App->gui->app_log.AddLog("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				App->gui->app_log.AddLog("Cannot fade in music %s. Mix_GetError(): %s\n", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				App->gui->app_log.AddLog("Cannot play in music %s. Mix_GetError(): %s\n", path, Mix_GetError());
				ret = false;
			}
		}
	}

	App->gui->app_log.AddLog("Successfully playing %s\n", path);
	return ret;
}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		App->gui->app_log.AddLog("Cannot load wav %s. Mix_GetError(): %s\n", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	Mix_Chunk* chunk = NULL;
	
	if(fx.at(id-1, chunk) == true)
	{
		Mix_PlayChannel(-1, chunk, repeat);
		ret = true;
	}

	return ret;
}