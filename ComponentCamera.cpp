#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(vec3 position, vec3 reference_offset)
{
	frustum = new Frustum();

	frustum->type = FrustumType::PerspectiveFrustum;
	frustum->pos = {position.x, position.y, position.z};

	frustum->front = { 0.0f, 0.0f, 1.0f};
	frustum->up    = { 0.0f, 1.0f, 0.0f};

	frustum->nearPlaneDistance = 1.0f;
	frustum->farPlaneDistance = 100.0f;

	frustum->horizontalFov = 90.0f * DEGTORAD;       //these angles provide an aspect ratio of 16:9
	frustum->verticalFov = 59.0f * DEGTORAD;

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = position;
	Reference = position + reference_offset;

}

ComponentCamera::~ComponentCamera()
{}

// -----------------------------------------------------------------
void ComponentCamera::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ComponentCamera::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ComponentCamera::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}


void ComponentCamera::RepositionToDisplay(ComponentAABB& aabb) {

	Move((vec3(aabb.GetMaxX(), aabb.GetMaxY(), aabb.GetMaxZ()) * REPOSITION_SCALE - Position));
	LookAt({ 0,0,0 });
}

