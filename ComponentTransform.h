#pragma once
#include "Component.h"

#include "MathGeoLib.h"


class ComponentTransform : Component {
public:

	ComponentTransform(float3 pos = { 0,0,0 }, float3 scl = { 0,0,0 }, Quat rot = { 0,0,0,0 });
	ComponentTransform(float3 euler_axis, float3 pos = { 0,0,0 }, float3 scl = { 0,0,0 });
	ComponentTransform(ComponentTransform& transform);

	~ComponentTransform();


	void SetPosition(float3 pos);
	void Translate(float3 dir);
	void SetScale(float3 scl);
	void Scale(float3 scl);
	void SetRotation(Quat rot);
	void SetRotationEuler(float3 euler_axis);
	void RotateAroundAxis(float3 axis, float rot_in_degrees);

	Component* Duplicate();
	void Update();

public:

	float3 position;
	float3 scale;
	Quat rotation;
	
	float4x4 mat;
};
