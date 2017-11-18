#include "ComponentAABB.h"

ComponentAABB::ComponentAABB() 
{
	type = COMPONENT_AABB;
	transform = new ComponentTransform();
}

ComponentAABB::ComponentAABB(float3* vert, int num_vert, ComponentTransform* parent_transform) {

	type = COMPONENT_AABB;

	Init(vert, num_vert);

	if (parent_transform)
		transform = new ComponentTransform(*parent_transform);
	else
		transform = new ComponentTransform();
}

ComponentAABB::ComponentAABB(ComponentMesh& mesh, ComponentTransform* parent_transform) {

	type = COMPONENT_AABB;

	Init(mesh.vertices, mesh.num_vertices);

	if (parent_transform)
		transform = new ComponentTransform(*parent_transform);
	else
		transform = new ComponentTransform();
}

ComponentAABB::ComponentAABB(GameObject& game_obj) {

	type = COMPONENT_AABB;

	std::vector<Component*> meshes = game_obj.FindComponents(COMPONENT_MESH);

	if (!meshes.empty())
		InitFromSeveralMeshes(meshes);
	else
		Init(nullptr, 0);

	if (ComponentTransform* game_obj_transform = (ComponentTransform*)game_obj.FindComponent(COMPONENT_TRANSFORM))
		transform = new ComponentTransform(*game_obj_transform);
	else
		transform = new ComponentTransform();
}



ComponentAABB::~ComponentAABB()
{
	if (vertices)
		delete[] vertices;
	if (obb)
		delete obb;
	if (transform)
		delete transform;
}

void ComponentAABB::Init(float3* vert, int num_vert) {

	vertices = new float3[BB_VERTNUM];

	if (vert != nullptr && num_vert != 0) {

		float max_x, min_x, max_y, min_y, max_z, min_z;

		max_x = min_x = vert[0].x;
		max_y = min_y = vert[0].y;
		max_z = min_z = vert[0].z;

		for (int i = 0; i < num_vert; i++) {

			min_x = (vert[i].x < min_x ? vert[i].x : min_x);
			max_x = (vert[i].x > max_x ? vert[i].x : max_x);

			min_y = (vert[i].y < min_y ? vert[i].y : min_y);
			max_y = (vert[i].y > max_y ? vert[i].y : max_y);

			min_z = (vert[i].z < min_z ? vert[i].z : min_z);
			max_z = (vert[i].z > max_z ? vert[i].z : max_z);

		}

		vertices[0].x = min_x; vertices[0].y = min_y; vertices[0].z = min_z;
		vertices[1].x = min_x; vertices[1].y = min_y; vertices[1].z = max_z;
		vertices[2].x = min_x; vertices[2].y = max_y; vertices[2].z = min_z;
		vertices[3].x = min_x; vertices[3].y = max_y; vertices[3].z = max_z;

		vertices[4].x = max_x; vertices[4].y = min_y; vertices[4].z = min_z;
		vertices[5].x = max_x; vertices[5].y = min_y; vertices[5].z = max_z;
		vertices[6].x = max_x; vertices[6].y = max_y; vertices[6].z = min_z;
		vertices[7].x = max_x; vertices[7].y = max_y; vertices[7].z = max_z;
	}
	else {
		for (int i = 0; i < BB_VERTNUM; i++)
			vertices[i] = { 0,0,0 };
		// LOG: error loading AABB vertices
	}

	obb = new ObjectBB(*this);
}

void ComponentAABB::InitFromSeveralMeshes(std::vector<Component*> meshes) {

	float3* total_vert = nullptr;
	int vert_count = 0;

	for (std::vector<Component*>::iterator it = meshes.begin(); it != meshes.end(); it++) {

		ComponentMesh* mesh = (ComponentMesh*)(*it);
		vert_count += mesh->num_vertices;

		if (total_vert) {

			float3* aux_vert_pointer = new float3[vert_count];

			for (int i = 0; i < vert_count - mesh->num_vertices; i++)
				aux_vert_pointer[i] = total_vert[i];
			for (int i = vert_count - mesh->num_vertices, j = 0; i < vert_count; i++, j++)
				aux_vert_pointer[i] = mesh->vertices[j];

			delete[] total_vert;
			total_vert = aux_vert_pointer;
		}
		else {
			total_vert = new float3[vert_count];

			for (int i = 0; i < vert_count; i++)
				total_vert[i] = mesh->vertices[i];
		}
	}

	Init(total_vert, vert_count);
}


void ComponentAABB::Save(JSON_file& save_file, const char* component_code)
{
	save_file.WriteNumber(std::string(".type").insert(0, component_code).c_str(), type);
	save_file.WriteFloat3(std::string(".min_point").insert(0, component_code).c_str(), GetMinP());
	save_file.WriteFloat3(std::string(".max_point").insert(0, component_code).c_str(), GetMaxP());

	transform->Save(save_file, std::string(".transform").insert(0, component_code).c_str());
}

void ComponentAABB::Load(JSON_file& save_file, const char* component_code)
{
	float3 min_max[2];
	min_max[0] = save_file.ReadFloat3(std::string(".min_point").insert(0, component_code).c_str());
	min_max[1] = save_file.ReadFloat3(std::string(".max_point").insert(0, component_code).c_str());

	transform->Load(save_file, std::string(".transform").insert(0, component_code).c_str());

	Init(min_max, 2);
}


void BoundingBox::Draw() {

	glLineWidth(2.0f);

	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_LINES);

	glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z); glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
	glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z); glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);

	glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z); glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
	glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z); glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
	glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z); glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
	glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z); glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);

	glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z); glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
	glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z); glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
	glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z); glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
	glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z); glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);

	glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z); glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
	glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z); glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
	glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z); glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
	glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z); glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glLineWidth(1.0f);

}


float3 BoundingBox::GetMinP() {

	float3 min_p = vertices[0];

	for (int i = 0; i < BB_VERTNUM; i++)
	{
		min_p.x = (vertices[i].x < min_p.x ? vertices[i].x : min_p.x);
		min_p.y = (vertices[i].y < min_p.y ? vertices[i].y : min_p.y);
		min_p.z = (vertices[i].z < min_p.z ? vertices[i].z : min_p.z);
	}

	return min_p;
}


float3 BoundingBox::GetMaxP() {

	float3 max_p = vertices[0];

	for (int i = 0; i < BB_VERTNUM; i++)
	{
		max_p.x = (vertices[i].x > max_p.x ? vertices[i].x : max_p.x);
		max_p.y = (vertices[i].y > max_p.y ? vertices[i].y : max_p.y);
		max_p.z = (vertices[i].z > max_p.z ? vertices[i].z : max_p.z);
	}

	return max_p;
}


ObjectBB::ObjectBB(ComponentAABB& parent_aabb) {

	vertices = new float3[BB_VERTNUM];
	float3 center = ((parent_aabb.GetMinP() + parent_aabb.GetMaxP()) / 2);

	for (int i = 0; i < BB_VERTNUM; i++) 
		vertices[i] = parent_aabb.vertices[i] - center;
	
}

