#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(float3 position)
{
	type = COMPONENT_CAMERA;

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

bool ComponentCamera::FrustumCulling(const ComponentAABB& aabb)
{
	Plane* planes = new Plane[6];
	frustum->GetPlanes(planes);

	for (int i = 0; i < 6; i++) {

		int vert_pos_side = 0;

		for (int j = 0; j < 8; j++) {
			float3 vertex = aabb.vertices[j];

			if (planes[i].IsOnPositiveSide(vertex))
				vert_pos_side++;
		}

		if (vert_pos_side == 8)
			return true;
	}

	return false;
}

Ray ComponentCamera::GetRayFromMousePos(float x_normalized, float y_normalized)
{
	return frustum->UnProjectLineSegment(x_normalized, y_normalized).ToRay();
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
	SetPosition(aabb.GetMaxP() * REPOSITION_SCALE);
	LookAt({ 0,0,0 });
}


void ComponentCamera::Save(JSON_file& save_file, const char* component_code)
{
	std::string attribute_code(component_code);

	save_file.WriteNumber(attribute_code.append(".type").c_str(), type);

	attribute_code.clear(); attribute_code.append(component_code);
	save_file.WriteNumber(attribute_code.append(".speed").c_str(), speed);

	attribute_code.clear(); attribute_code.append(component_code);
	save_file.WriteNumber(attribute_code.append(".sensitivity").c_str(), sensitivity);

	attribute_code.clear(); attribute_code.append(component_code);
	save_file.WriteNumber(attribute_code.append(".near_plane").c_str(), frustum->NearPlaneDistance());

	attribute_code.clear(); attribute_code.append(component_code);
	save_file.WriteNumber(attribute_code.append(".far_plane").c_str(), frustum->FarPlaneDistance());

	attribute_code.clear(); attribute_code.append(component_code);
	save_file.WriteNumber(attribute_code.append(".horizontal_fov").c_str(), frustum->HorizontalFov());

	attribute_code.clear(); attribute_code.append(component_code);
	save_file.WriteNumber(attribute_code.append(".vertical_fov").c_str(), frustum->VerticalFov());

	attribute_code.clear(); attribute_code.append(component_code); attribute_code.append(".transform");
	transform->Save(save_file, attribute_code.c_str());

}
void ComponentCamera::Load(JSON_file& save_file, const char* component_code)
{
	std::string attribute_code(component_code);

	speed = save_file.ReadNumber(attribute_code.append(".speed").c_str());

	attribute_code.clear(); attribute_code.append(component_code);
	sensitivity = save_file.ReadNumber(attribute_code.append(".sensitivity").c_str());

	attribute_code.clear(); attribute_code.append(component_code);
	near_plane_d = save_file.ReadNumber(attribute_code.append(".near_plane").c_str());

	attribute_code.clear(); attribute_code.append(component_code);
	far_plane_d = save_file.ReadNumber(attribute_code.append(".far_plane").c_str());

	attribute_code.clear(); attribute_code.append(component_code);
	horizontal_fov = save_file.ReadNumber(attribute_code.append(".horizontal_fov").c_str());

	attribute_code.clear(); attribute_code.append(component_code);
	vertical_fov = save_file.ReadNumber(attribute_code.append(".vertical_fov").c_str());

	attribute_code.clear(); attribute_code.append(component_code); attribute_code.append(".transform");
	transform->Load(save_file, attribute_code.c_str());
}

