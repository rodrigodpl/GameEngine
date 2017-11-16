#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentCamera.h"

#include "glew-2.1.0\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#pragma comment( lib, "glew-2.1.0/lib/Release/Win32/glew32.lib")
#pragma comment( lib, "glew-2.1.0/lib/Release/Win32/glew32s.lib")


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JSON_file& config)
{
	App->gui->app_log.AddLog("Creating 3D Renderer context\n");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		App->gui->app_log.AddLog("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit();
	App->gui->app_log.AddLog("Using Glew %s\n", glewGetString(GLEW_VERSION));

	App->gui->app_log.AddLog("Vendor: %s\n", glGetString(GL_VENDOR));
	App->gui->app_log.AddLog("Renderer: %s\n", glGetString(GL_RENDERER));
	App->gui->app_log.AddLog("OpenGL version supported %s\n", glGetString(GL_VERSION));
	App->gui->app_log.AddLog("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	//  (config.ReadBool("renderer.vsync")
	if(ret)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->gui->app_log.AddLog("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->gui->app_log.AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->gui->app_log.AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		//Color background_color = config.ReadColor("renderer.background_color");
		//glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
		glClearColor(0.9f, 0.9f, 0.9f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->gui->app_log.AddLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		//Color ambient_light = config.ReadColor("renderer.ambient_light");
		GLfloat LightModelAmbient[] = { 0.5, 0.5, 0.5, 1.0 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	dt = (float)ms_timer.Read() / 1000.0f;
	App->gui->Fps_renderer_data(dt);
	ms_timer.Start();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if (App->camera->cam->proj_update) {

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(App->camera->cam->ProjectionMatrix());
		App->camera->cam->proj_update = false;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->cam->ViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->cam->transform->position.x, App->camera->cam->transform->position.y, App->camera->cam->transform->position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//first draw scene
	App->scene_intro->DrawScene();

	//then debug features
	if (debug_draw)
		DebugDraw();
	
	//and editor last
	App->gui->Draw();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->gui->app_log.AddLog("Destroying 3D Renderer\n");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DebugDraw() {}


	//Direct draw of a cube using TRIS
	/*glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.f, 1.f, 0.f); //C
		glVertex3f(0.f, 0.f, 0.f); //A
		glVertex3f(1.f, 0.f, 0.f); //B

		glVertex3f(0.f, 1.f, 0.f); //C
		glVertex3f(1.f, 0.f, 0.f); //B
		glVertex3f(1.f, 1.f, 0.f); //D

		glVertex3f(1.f, 1.f, 0.f); //D
		glVertex3f(1.f, 0.f, 0.f); //B
		glVertex3f(1.f, 0.f, -1.f); //F

		glVertex3f(1.f, 1.f, 0.f); //D
		glVertex3f(1.f, 0.f, -1.f); //F
		glVertex3f(1.f, 1.f, -1.f); //H

		glVertex3f(0.f, 1.f, -1.f); //G
		glVertex3f(0.f, 1.f, 0.f); //C
		glVertex3f(1.f, 1.f, 0.f); //D

		glVertex3f(0.f, 1.f, -1.f); //G
		glVertex3f(1.f, 1.f, 0.f); //D
		glVertex3f(1.f, 1.f, -1.f); //H

		glVertex3f(1.f, 1.f, -1.f); //H
		glVertex3f(1.f, 0.f, -1.f); //F
		glVertex3f(0.f, 0.f, -1.f); //E

		glVertex3f(0.f, 1.f, -1.f); //G
		glVertex3f(1.f, 1.f, -1.f); //H
		glVertex3f(0.f, 0.f, -1.f); //E

		glVertex3f(0.f, 0.f, 0.f); //A
		glVertex3f(0.f, 0.f, -1.f); //E
		glVertex3f(1.f, 0.f, 0.f); //B
		
		glVertex3f(0.f, 0.f, -1.f); //E
		glVertex3f(1.f, 0.f, -1.f); //F
		glVertex3f(1.f, 0.f, 0.f); //B

		glVertex3f(0.f, 1.f, 0.f); //C
		glVertex3f(0.f, 0.f, -1.f); //E
		glVertex3f(0.f, 0.f, 0.f); //A

		glVertex3f(0.f, 1.f, 0.f); //C
		glVertex3f(0.f, 1.f, -1.f); //G
		glVertex3f(0.f, 0.f, -1.f); //E
	glEnd();
	*/

	/*  Coordinates cube
		glVertex3f(0.f, 0.f, 0.f); //A
		glVertex3f(1.f, 0.f, 0.f); //B
		glVertex3f(0.f, 1.f, 0.f); //C
		glVertex3f(1.f, 1.f, 0.f); //D
		glVertex3f(0.f, 0.f, -1.f); //E
		glVertex3f(1.f, 0.f, -1.f); //F
		glVertex3f(0.f, 1.f, -1.f); //G
		glVertex3f(1.f, 1.f, -1.f); //H
	
	glLineWidth(1.0f);*/

	
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//// … draw other buffers
	//glDrawArrays(GL_TRIANGLES, 0, VERT_NUM * 3);
	//glDisableClientState(GL_VERTEX_ARRAY);
