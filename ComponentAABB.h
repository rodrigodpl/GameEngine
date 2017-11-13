#pragma once
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"

#define BB_VERTNUM 24

class ObjectBB;

class ComponentAABB : public Component {

public:
	float* vertices = nullptr;
	ObjectBB* obb = nullptr;                     // obb is created in Init method
	ComponentTransform* transform;

public:

	ComponentAABB(float* vert, int num_vert, ComponentTransform* parent_transform = nullptr);
	ComponentAABB(ComponentMesh& mesh, ComponentTransform* parent_transform = nullptr);
	ComponentAABB(GameObject& game_obj);
	~ComponentAABB();

	void Init(float* vert, int num_vert);
	void InitFromSeveralMeshes(std::vector<Component*> meshes);

	float GetMinX(); float GetMaxX();
	float GetMinY(); float GetMaxY();
	float GetMinZ(); float GetMaxZ();

	void Draw();

	AABB GetMatGeoLibAABB();

};

class ObjectBB {

public:
	float* vertices = nullptr;

public:
	ObjectBB(ComponentAABB& parent_aabb);

};