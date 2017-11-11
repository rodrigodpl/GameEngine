#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleEditorCam.h"
#include "glmath.h"

#include "MathGeoLib.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

#ifdef _DEBUG
#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif


ModuleEditorCam::ModuleEditorCam(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleEditorCam::~ModuleEditorCam()
{}

bool ModuleEditorCam::Start() {

	cam = new ComponentCamera({ 0, 2, -5 });
	GameObject* cam_game_obj = new GameObject("Main_Camera");

	cam_game_obj->components.push_back(cam);
	App->scene_intro->game_objects.push_back(cam_game_obj);

	return cam;
}

// ----------------------------------------------------------------
bool ModuleEditorCam::CleanUp()
{
	if(cam)
		delete cam;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleEditorCam::Update(float dt)
{
	float dx = 0;  float dy = 0;  float dz = 0;

	// Look at (0,0,0)
	if (App->input->GetKey(SDL_SCANCODE_F))
		cam->LookAt({ 0,0,0 });

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) {

		//mouse motion
		dx = -App->input->GetMouseXMotion();
		dy = -App->input->GetMouseYMotion();

		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)      cam->LookAround(dx, dy);    // look around							
		else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)  cam->Orbit(dx, dy);	       // orbit around reference

		dx = dy = 0;
	}

	float speed = 3.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 6.0f * dt;

	// key motion
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) dz += speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) dz -= speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) dx += speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) dx -= speed;

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) dy += speed;
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) dy -= speed;

	// Movement in camera axis
	cam->Move(dx, dy, dz);

	// Zoom in/out
	cam->Zoom(App->input->GetMouseZ());

	return UPDATE_CONTINUE;
}
