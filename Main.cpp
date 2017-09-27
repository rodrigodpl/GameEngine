#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{	
		switch (state)
		{
		case MAIN_CREATION:

			App = new Application();
			state = MAIN_START;
			App->gui->app_log.AddLog("-------------- Application Creation --------------\n");
			break;

		case MAIN_START:

			App->gui->app_log.AddLog("-------------- Application Init --------------\n");
			if (App->Init() == false)
			{
				App->gui->app_log.AddLog("Application Init exits with ERROR\n");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				App->gui->app_log.AddLog("-------------- Application Update --------------\n");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				App->gui->app_log.AddLog("Application Update exits with ERROR\n");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			App->gui->app_log.AddLog("-------------- Application CleanUp --------------\n");
			if (App->CleanUp() == false)
			{
				App->gui->app_log.AddLog("Application CleanUp exits with ERROR\n");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			App->gui->app_log.AddLog("Exiting game '%s'...\n", TITLE);
			break;

		}
	}

	delete App;
	return main_return;
}