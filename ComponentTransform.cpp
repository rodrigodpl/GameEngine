#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(Quat rot , float3 pos, float3 scl) : position(pos), scale(scl), rotation(rot)
{
	type = Component_type::COMPONENT_TRANSFORM;
	mat = mat.FromTRS(position, rotation, scale);
}

ComponentTransform::ComponentTransform(float3 euler_axis, float3 pos, float3 scl) : position(pos), scale(scl)
{
	type = Component_type::COMPONENT_TRANSFORM;
	rotation = rotation.FromEulerXYZ(euler_axis.x, euler_axis.y, euler_axis.z);
	mat = mat.FromTRS(position, rotation, scale);
}

ComponentTransform::ComponentTransform(ComponentTransform& transform) : position(transform.position), scale(transform.scale), rotation(transform.rotation) 
{
	type = Component_type::COMPONENT_TRANSFORM;
	mat = (transform.mat);
}

ComponentTransform::ComponentTransform() 
{
	type = Component_type::COMPONENT_TRANSFORM;
}

ComponentTransform::~ComponentTransform() 
{}

void ComponentTransform::SetPosition(float3 pos)
{
	mat = mat.FromTRS(pos, rotation, scale);
	position = pos;
}

void ComponentTransform::Translate(float3 dir) {

	mat = mat.FromTRS(position + dir, rotation, scale);
	position += dir;

}

void ComponentTransform::SetRotation(Quat rot)
{
	mat = mat.FromTRS(position, rot, scale);
	rotation = rot; 
}

void ComponentTransform::RotateAroundAxis(float3 axis, float rot_in_degrees) 
{
	mat = mat.RotateAxisAngle(axis, DegToRad(rot_in_degrees));
	float3 euler_axis = mat.ToEulerXYZ();
	rotation = rotation.FromEulerXYZ(euler_axis.x, euler_axis.y, euler_axis.z);
}

void ComponentTransform::SetRotationEuler(float3 euler_axis) {
	
	Quat rot = rot.FromEulerXYZ(euler_axis.x, euler_axis.y, euler_axis.z);
	mat = mat.FromTRS(position, rot, scale);
	rotation = rot;
}

void ComponentTransform::SetScale(float3 scl)
{
	mat = mat.FromTRS(position, rotation, scl);
	scale = scl;
}

void ComponentTransform::Scale(float3 scl) {

	mat = mat.FromTRS(position, rotation, scale + scl);
	scale += scl;

}

Component* ComponentTransform::Duplicate()
{
	return (Component*) new ComponentTransform(*this);
}

void ComponentTransform::LookAt(float3 position, float3 target, float3 forward, float3 up, float3 worldUp) 
{
	mat = mat.LookAt(position, target, forward, up, worldUp);
	this->position = position;
	float3 euler_axis = mat.ToEulerXYZ();
	rotation = rotation.FromEulerXYZ(euler_axis.x, euler_axis.y, euler_axis.z);
}

void ComponentTransform::Update() 
{}


void ComponentTransform::Save(JSON_file& save_file, const char* component_code)
{
	std::string attribute_code(component_code);

	save_file.WriteNumber(std::string(".type").insert(0, component_code).c_str(), type);
	save_file.WriteFloat3(std::string(".position").insert(0, component_code).c_str(), position);
	save_file.WriteFloat3(std::string(".scale").insert(0, component_code).c_str(),	scale);
	save_file.WriteFloat3(std::string(".rotation_euler").insert(0, component_code).c_str(), rotation.ToEulerXYZ());
}

void ComponentTransform::Load(JSON_file& save_file, const char* component_code)
{
	position = save_file.ReadFloat3(std::string(".position").insert(0, component_code).c_str());
	scale = save_file.ReadFloat3(std::string(".scale").insert(0, component_code).c_str());
	float3 rotation_euler = save_file.ReadFloat3(std::string(".rotation_euler").insert(0, component_code).c_str());

	rotation.FromEulerXYZ(rotation_euler.x, rotation_euler.y, rotation_euler.z);
	mat.FromTRS(position, rotation, scale);
}

