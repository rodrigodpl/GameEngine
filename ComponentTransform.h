#pragma once
#include "Component.h"

#include "MathGeoLib.h"


class ComponentTransform : Component {
public:

	ComponentTransform(Quat rot = { 0,0,0,0 },float3 pos = { 0,0,0 }, float3 scl = { 1,1,1 } );
	ComponentTransform(float3 euler_axis, float3 pos = { 0,0,0 }, float3 scl = { 1,1,1 });
	ComponentTransform(ComponentTransform& transform);

	~ComponentTransform();


	void SetPosition(float3 pos);
	void Translate(float3 dir);
	void SetScale(float3 scl);
	void Scale(float3 scl);
	void SetRotation(Quat rot);
	void SetRotationEuler(float3 euler_axis);
	void RotateAroundAxis(float3 axis, float rot_in_degrees);

	void LookAt(float3 position, float3 target, float3 forward, float3 up, float3 worldUp);

	Component* Duplicate();
	void Update();

public:

	float3 position;
	float3 scale;
	Quat rotation;
	
	float4x4 mat;
};
