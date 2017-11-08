#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleEditorCam.h"
#include "glmath.h"

#include "MathGeoLib-1.5\src\MathGeoLib.h"
#include "MathGeoLib-1.5\src\Geometry\AABB.h"

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

// -----------------------------------------------------------------
bool ModuleEditorCam::Start()
{
	cam = new ComponentCamera();
	GameObject* cam_game_obj = new GameObject("Main_Camera");

	cam_game_obj->components.push_back(cam);
	App->scene_intro->game_objects.push_back(cam_game_obj);

	return (cam);
}

// -----------------------------------------------------------------
bool ModuleEditorCam::CleanUp()
{
	if(cam)
		delete cam;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleEditorCam::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0,0,0);
	float speed = 3.0f * dt;

	// Mouse motion ----------------

	if (App->input->GetKey(SDL_SCANCODE_F))
		cam->LookAt(vec3(0, 0, 0));


	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		// look around

		btVector3 VecPostoRef(cam->Reference.x - cam->Position.x, cam->Reference.y - cam->Position.y, cam->Reference.z - cam->Position.z);

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.015f;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			VecPostoRef = VecPostoRef.rotate(btVector3(0, 1, 0), DeltaX);
		}
		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;
			
			btVector3 axis(cam->X.x, cam->X.y, cam->X.y);
			VecPostoRef = VecPostoRef.rotate(axis.normalized(), DeltaY);
		}


		cam->Reference = { cam->Position.x + VecPostoRef.x(), cam->Position.y + VecPostoRef.y(), cam->Position.z + VecPostoRef.z()};
		cam->LookAt(cam->Reference);

	}
	else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		// orbit around reference

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		cam->Position -= cam->Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			cam->X = rotate(cam->X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			cam->Y = rotate(cam->Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			cam->Z = rotate(cam->Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			cam->Y = rotate(cam->Y, DeltaY, cam->X);
			cam->Z = rotate(cam->Z, DeltaY, cam->X);

			if (cam->Y.y < 0.0f)
			{
				cam->Z = vec3(0.0f, cam->Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				cam->Y = cross(cam->Z, cam->X);
			}
		}

		cam->Position = cam->Reference + cam->Z * length(cam->Position);

	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= cam->Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += cam->Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= cam->X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += cam->X * speed;

	cam->Position += newPos;
	cam->Reference += newPos;

	if (App->input->GetMouseZ() > 0)                // zoom in
		cam->Position += (cam->Reference - cam->Position) * 0.1f;
	else if (App->input->GetMouseZ() < 0)           // zoom out
		cam->Position += (cam->Position - cam->Reference) * 0.1f;


	// Recalculate matrix -------------
	cam->CalculateViewMatrix();

	return UPDATE_CONTINUE;
}
