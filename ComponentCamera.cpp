#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(float3 position)
{
	frustum = new Frustum();

	frustum->SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum->SetViewPlaneDistances(0.5f, 100.0f);
	frustum->SetFrame(position, { 0,0,1 }, { 0,1,0 });
	frustum->SetPerspective(90.0f * DEGTORAD, 59.0f * DEGTORAD);                       //these angles provide an aspect ratio of 16:9
	
	transform = new ComponentTransform(Quat::identity, { position.x, position.y, position.z });

	ref_dist = INITIAL_REF_DISTANCE;
	reference = transform->position + (frustum->Front() * ref_dist);

	proj_update = true;
}

ComponentCamera::~ComponentCamera()
{
	delete frustum;
}

void ComponentCamera::LookAt(const float3 &Spot)
{
	frustum->Transform(transform->mat.Inverted());
	transform->LookAt(transform->position, Spot, frustum->Front(), frustum->Up(), { 0,1,0 });
	frustum->Transform(transform->mat);
	frustum->WorldMatrixChanged();

	reference = Spot;
	ref_dist = Spot.Distance(transform->position);
}


void ComponentCamera::LookAround(float dx, float dy) 
{
	if (dx != 0 || dy != 0) {
		float3 new_dir = (reference - transform->position);
		new_dir = Quat::FromEulerXYZ(0, dx * DEGTORAD * sensitivity, 0) * new_dir;  // rotation over y axis
		new_dir = Quat::FromEulerXYZ(-dy * DEGTORAD * sensitivity, 0, 0) * new_dir;  // rotation over x axis
		LookAt(transform->position + new_dir);
	}
}

void ComponentCamera::Orbit(float dx, float dy) {

	float3 ref_to_curr_pos = transform->position - reference;
	Quat rot = rot.FromEulerXYZ(-dy * DEGTORAD * sensitivity, dx * DEGTORAD * sensitivity, 0);

	float3 ref_saved = reference;
	SetPosition(reference + (rot * ref_to_curr_pos));                      // (rot * ref_to_curr_pos) = ref to new position
	LookAt(ref_saved);
}

void ComponentCamera::Move(const float3 &Movement)
{
	frustum->Transform(transform->mat.Inverted());
	transform->Translate(Movement);
	frustum->Transform(transform->mat);
	frustum->WorldMatrixChanged();

	reference += Movement;
}

void ComponentCamera::Move(float dx, float dy, float dz)
{
	if (dx != 0 || dy != 0 || dz != 0) {
		float3 delta_x = (Quat::RotateAxisAngle(frustum->Up(), 90 * DEGTORAD) * frustum->Front()) * (dx * speed);
		float3 delta_y = frustum->Up() * (dy * speed);
		float3 delta_z = frustum->Front() * (dz * speed);

		Move(delta_x + delta_y + delta_z);
	}
}

void ComponentCamera::Zoom(float delta) 
{
	if (delta != 0 && ref_dist + delta > 0) {
		float3 ref_saved = reference;
		ref_dist = ref_dist + (delta * (ref_dist * 0.1));
		SetPosition(reference + ((transform->position - reference).Normalized() * ref_dist));
		LookAt(ref_saved);
	}
}

float* ComponentCamera::ViewMatrix()
{ 
	float4x4 mat = frustum->ViewMatrix();
	return mat.Transposed().ptr();
}

float* ComponentCamera::ProjectionMatrix()
{
	return frustum->ProjectionMatrix().ptr();
}

void ComponentCamera::SetPosition(const float3& position)
{
	Move(position - transform->position);
}

void ComponentCamera::SetSpeed(float speed)
{
	this->speed = speed;
}

void ComponentCamera::SetSensitivity(float sensitivity)
{
	this->sensitivity = sensitivity;
}

void ComponentCamera::ChangeFOV(float vertical_FOV)
{
	frustum->SetVerticalFovAndAspectRatio(vertical_FOV, frustum->AspectRatio());
	proj_update = true;
}
void ComponentCamera::ChangeAspectRatio(float aspect_ratio)
{
	frustum->SetVerticalFovAndAspectRatio(frustum->VerticalFov(), aspect_ratio);
	proj_update = true;
}

void ComponentCamera::RepositionToDisplay(ComponentAABB& aabb) 
{
	SetPosition(float3(aabb.GetMaxX(), aabb.GetMaxY(), aabb.GetMaxZ()) * REPOSITION_SCALE);
	LookAt({ 0,0,0 });
}

