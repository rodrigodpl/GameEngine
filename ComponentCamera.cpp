#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(vec3 position)
{
	frustum = new Frustum();

	frustum->SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum->SetViewPlaneDistances(0.5f, 100.0f);
	frustum->SetFrame({ position.x, position.y, position.z }, { 0,0,1 }, { 0,1,0 });
	frustum->SetPerspective(90.0f * DEGTORAD, 59.0f * DEGTORAD);                       //these angles provide an aspect ratio of 16:9

	transform = new ComponentTransform(Quat::identity, { position.x, position.y, position.z });
	//frustum->Transform(transform->mat);
}

ComponentCamera::~ComponentCamera()
{}

// -----------------------------------------------------------------
void ComponentCamera::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	transform->SetPosition({ Position.x, Position.y, Position.z });
	//transform->LookAt(transform->position, { Reference.x, Reference.y, Reference.z }, frustum->front, frustum->up, { 0,1,0 });
	frustum->Transform(transform->mat);
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const vec3 &Spot)
{
	//transform->LookAt(transform->position, { Spot.x, Spot.y, Spot.z }, frustum->front, frustum->up, { 0,1,0 });
	frustum->Transform(transform->mat);
}


// -----------------------------------------------------------------
void ComponentCamera::Move(const float3 &Movement)
{
	transform->Translate({ Movement.x, Movement.y, Movement.z });
	frustum->Transform(transform->mat);
}

// -----------------------------------------------------------------
float* ComponentCamera::ViewMatrix()
{ 
	float4x4 mat = frustum->ViewMatrix();
	return mat.Transposed().ptr();
}

float* ComponentCamera::ProjectionMatrix()
{
	return frustum->ProjectionMatrix().ptr();
}



void ComponentCamera::RepositionToDisplay(ComponentAABB& aabb) 
{
	Move((float3(aabb.GetMaxX(), aabb.GetMaxY(), aabb.GetMaxZ()) * REPOSITION_SCALE - transform->position));
	LookAt({ 0,0,0 });
}

