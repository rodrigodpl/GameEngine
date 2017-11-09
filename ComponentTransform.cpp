#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(float3 pos, float3 scl, Quat rot) : position(pos), scale(scl), rotation(rot)
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

void ComponentTransform::Update() 
{}


