#pragma once
#include "Module.h"
#include "Globals.h"
#include "ComponentMesh.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void FBXRescalePosition(ComponentMesh& mesh);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	float rescale_value;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;

private:
	Timer	ms_timer;
	float	dt;
};