#pragma once
#include "Timer.h"
#include "ComponentMesh.h"
#include "glmath.h"
#include "ComponentAABB.h"

#include "MathGeoLib.h"

#define REPOSITION_SCALE 1.4f

class ComponentCamera : public Component
{
public:
	ComponentCamera(vec3 position = { 0.0f, 2.0f, 5.0f }, vec3 reference_offset = { 0.0f, 0.0f, 5.0f });
	~ComponentCamera();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void RepositionToDisplay(ComponentAABB& mesh);
	void ChangeFOV(float vertical_FOV);
	void ChangeAspectRatio(float aspect_ratio);
	float* GetViewMatrix();

	void CalculateViewMatrix();

public:

	vec3 X, Y, Z, Position, Reference;
	Frustum* frustum = nullptr;

private:

	float aspect_ratio;
	float vert_fow;
	mat4x4 ViewMatrix, ViewMatrixInverse;
};