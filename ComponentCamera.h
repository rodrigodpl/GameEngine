#pragma once
#include "Timer.h"
#include "ComponentMesh.h"
#include "glmath.h"
#include "ComponentAABB.h"
#include "ComponentTransform.h"

#include "MathGeoLib.h"

#define REPOSITION_SCALE 1.4f
#define INITIAL_REF_DISTANCE 5.0f

class ComponentCamera : public Component
{
public:
	ComponentCamera(float3 position = { 0.0f, -2.0f, -5.0f });
	~ComponentCamera();

	void LookAt(const float3 &Spot);
	void LookAround(float dx, float dy);
	void Orbit(float dx, float dy);
	void Zoom(float delta);

	void SetPosition(const float3& position);
	void Move(const float3 &Movement);           // uses world axis but does not use speed. utility-targeted method
	void Move(float dx, float dy, float dz);     // uses local axis and speed.              user-targeted method
	void RepositionToDisplay(ComponentAABB& mesh);

	void ChangeFOV(float vertical_FOV);
	void ChangeAspectRatio(float aspect_ratio);
	void SetSpeed(float speed);
	void SetSensitivity(float sensitivity);

	float* ViewMatrix();
	float* ProjectionMatrix();

public:

	ComponentTransform* transform = nullptr;
	bool proj_update = false;

private:

	Frustum* frustum = nullptr;

	float speed = 1.0f;
	float sensitivity = 0.25f;

	float3 reference = { 0,0,0 };
	float ref_dist = 0.0f;
};