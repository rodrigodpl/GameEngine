#pragma once
#include "Component.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

class ComponentTransform : Component {
public:

	ComponentTransform(aiVector3D pos = { 0,0,0 }, aiVector3D scl = { 0,0,0 }, aiQuaternion rot = { 0,0,0,0 });
	ComponentTransform(ComponentTransform& transform);

	~ComponentTransform();


	void SetPosition(aiVector3D pos);
	void SetScale(aiVector3D scl);
	void SetRotation(aiQuaternion rot);

	Component* Duplicate();
	void Update();

public:

	aiVector3D position;
	aiVector3D scale;
	aiQuaternion rotation;
};
