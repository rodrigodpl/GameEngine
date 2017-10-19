#include "ComponentTransform.h"


ComponentTransform::ComponentTransform(aiVector3D pos, aiVector3D scl, aiQuaternion rot) : position(pos), scale(scl), rotation(rot)
{
	type = Component_type::COMPONENT_TRANSFORM;
}

ComponentTransform::ComponentTransform(ComponentTransform& transform) {

	type = Component_type::COMPONENT_TRANSFORM;

	position = transform.position;
	scale = transform.scale;
	rotation = transform.rotation;
}

ComponentTransform::~ComponentTransform() 
{}


void ComponentTransform::SetPosition(aiVector3D pos)
{
	position = pos;
}

void ComponentTransform::SetScale(aiVector3D scl)
{
	scale = scl;
}

void ComponentTransform::SetRotation(aiQuaternion rot)
{
	rotation = rot; 
}

Component* ComponentTransform::Duplicate()
{
	return (Component*) new ComponentTransform(*this);
}

void ComponentTransform::Update() 
{}


