#pragma once
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "GameObject.h"

#define BB_VERTNUM 8

class ObjectBB;

class BoundingBox : public Component {
public:
	float3 GetMinP(); float3 GetMaxP();

	void Draw();

public:

	float3* vertices = nullptr;						// scene saved variable

};

class ComponentAABB : public BoundingBox {
public:

	ComponentAABB(float3* vert, int num_vert, ComponentTransform* parent_transform = nullptr);
	ComponentAABB(ComponentMesh& mesh, ComponentTransform* parent_transform = nullptr);
	ComponentAABB(GameObject& game_obj);
	ComponentAABB();									// empty constructor! Load MUST be called afterwards
	~ComponentAABB();

	void Init(float3* vert, int num_vert);
	void InitFromSeveralMeshes(std::vector<Component*> meshes);

	void Save(JSON_file& save_file, const char* component_code);
	void Load(JSON_file& save_file, const char* component_code);

public:
	ObjectBB* obb = nullptr;                     // obb is created in Init method
	ComponentTransform* transform = nullptr;

};

class ObjectBB : BoundingBox {

public:
	ObjectBB(ComponentAABB& parent_aabb);

};
