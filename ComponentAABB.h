#pragma once
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"

#define BB_VERTNUM 24

class ObjectBB;

class BoundingBox : public Component {
public:

	void Save(JSON_file& save_file, const char* component_code);
	//void Load(JSON_file& save_file, const char* component_code);

	float GetMinX(); float GetMaxX();
	float GetMinY(); float GetMaxY();
	float GetMinZ(); float GetMaxZ();

	void Draw();

public:
	float* vertices = nullptr;

};

class ComponentAABB : public BoundingBox {
public:

	ComponentAABB(float* vert, int num_vert, ComponentTransform* parent_transform = nullptr);
	ComponentAABB(ComponentMesh& mesh, ComponentTransform* parent_transform = nullptr);
	ComponentAABB(GameObject& game_obj);
	ComponentAABB(float3 min_p, float3 max_p);
	~ComponentAABB();

	void Init(float* vert, int num_vert);
	void InitFromSeveralMeshes(std::vector<Component*> meshes);

public:
	ObjectBB* obb = nullptr;                     // obb is created in Init method
	ComponentTransform* transform;

};

class ObjectBB : BoundingBox {

public:
	float* vertices = nullptr;

public:
	ObjectBB(ComponentAABB& parent_aabb);

};
