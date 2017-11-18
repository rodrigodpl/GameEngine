#pragma once
#include "Component.h"

#include "MathGeoLib.h"


class ComponentTransform : Component {
public:

	ComponentTransform(Quat rot ,float3 pos, float3 scl);
	ComponentTransform(float3 euler_axis, float3 pos, float3 scl);
	ComponentTransform(ComponentTransform& transform);
	ComponentTransform();											// empty constructor! Load MUST be called afterwards

	~ComponentTransform();

	void Save(JSON_file& save_file, const char* component_code);
	void Load(JSON_file& save_file, const char* component_code);

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

	float3 position = { 0,0,0 };			// scene saved variable
	float3 scale = { 1,1,1 };				// scene saved variable
	Quat rotation = { 0,0,0,0 };			// scene saved variable
	
	float4x4 mat = mat.identity;
};
