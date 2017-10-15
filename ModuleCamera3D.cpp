#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "glmath.h"

#include "MathGeoLib-1.5\src\MathGeoLib.h"
#include "MathGeoLib-1.5\src\Geometry\AABB.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	rescale_value = 1.4f;
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	App->gui->app_log.AddLog("Setting up the camera\n");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	App->gui->app_log.AddLog("Cleaning camera\n");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0,0,0);
	float speed = 3.0f * dt;

	// Mouse motion ----------------

	if (App->input->GetKey(SDL_SCANCODE_F))
		LookAt(vec3(0, 0, 0));


	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		// look around

		btVector3 VecPostoRef(Reference.x - Position.x, Reference.y - Position.y, Reference.z - Position.z);

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
			
			btVector3 axis(X.x, X.y, X.y);
			VecPostoRef = VecPostoRef.rotate(axis.normalized(), DeltaY);
		}


		Reference = { Position.x + VecPostoRef.x(), Position.y + VecPostoRef.y(), Position.z + VecPostoRef.z()};
		LookAt(Reference);

	}
	else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		// orbit around reference

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);

	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	if (App->input->GetMouseZ() > 0)                // zoom in
		Position += (Reference - Position) * 0.1f;
	else if (App->input->GetMouseZ() < 0)           // zoom out
		Position += (Position - Reference) * 0.1f;


	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}


void ModuleCamera3D::FBXRescalePosition(Mesh& mesh) {

	float max_x, min_x, max_y, min_y, max_z, min_z;
	max_x = min_x = max_y = min_y = max_z = min_z = 0.0f;

	for (int i = 0; i < mesh.num_vertices * 3; i += 3) {

		max_x = (max_x < mesh.vertices[i] ? mesh.vertices[i] : max_x); 
		max_y = (max_y < mesh.vertices[i + 1] ? mesh.vertices[i + 1] : max_y); 
		max_z = (max_z < mesh.vertices[i + 2] ? mesh.vertices[i + 2] : max_z); 
	}

	Move((vec3(max_x, max_y, max_z) * rescale_value) - Position);
	LookAt({ 0,0,0 });
}

