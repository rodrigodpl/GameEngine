#include "ComponentAABB.h"

ComponentAABB::ComponentAABB(float* vert, int num_vert, ComponentTransform* parent_transform) {

	type = COMPONENT_AABB;

	Init(vert, num_vert);

	if (parent_transform)
		transform = new ComponentTransform(*parent_transform);
	else
		transform = new ComponentTransform();
}

ComponentAABB::ComponentAABB(ComponentMesh& mesh, ComponentTransform* parent_transform) {

	type = COMPONENT_AABB;

	Init(mesh.vertices, mesh.num_vertices * 3);

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

void ComponentAABB::Init(float* vert, int num_vert) {

	vertices = new float[BB_VERTNUM];

	if (vert != nullptr && num_vert != 0) {

		float max_x, min_x, max_y, min_y, max_z, min_z;

		max_x = min_x = vert[0];
		max_y = min_y = vert[1];
		max_z = min_z = vert[2];

		for (int i = 3; i < num_vert; i += 3) {

			min_x = (vert[i] < min_x ? vert[i] : min_x);
			max_x = (vert[i] > max_x ? vert[i] : max_x);

			min_y = (vert[i + 1] < min_y ? vert[i + 1] : min_y);
			max_y = (vert[i + 1] > max_y ? vert[i + 1] : max_y);

			min_z = (vert[i + 2] < min_z ? vert[i + 2] : min_z);
			max_z = (vert[i + 2] > max_z ? vert[i + 2] : max_z);

		}

		vertices[0] = min_x; vertices[1] = min_y; vertices[2] = min_z;
		vertices[3] = min_x; vertices[4] = min_y; vertices[5] = max_z;
		vertices[6] = min_x; vertices[7] = max_y; vertices[8] = min_z;
		vertices[9] = min_x; vertices[10] = max_y; vertices[11] = max_z;

		vertices[12] = max_x; vertices[13] = min_y; vertices[14] = min_z;
		vertices[15] = max_x; vertices[16] = min_y; vertices[17] = max_z;
		vertices[18] = max_x; vertices[19] = max_y; vertices[20] = min_z;
		vertices[21] = max_x; vertices[22] = max_y; vertices[23] = max_z;
	}
	else {
		for (int i = 0; i < BB_VERTNUM; i++)
			vertices[i] = 0;
		// LOG: error loading AABB vertices
	}

	obb = new OBB(*this);
}

void ComponentAABB::InitFromSeveralMeshes(std::vector<Component*> meshes) {

	float* total_vert = nullptr;
	int vert_count = 0;

	for (std::vector<Component*>::iterator it = meshes.begin(); it != meshes.end(); it++) {

		ComponentMesh* mesh = (ComponentMesh*)(*it);
		vert_count += mesh->num_vertices * 3;

		if (total_vert) {

			float* aux_vert_pointer = new float[vert_count];

			for (int i = 0; i < vert_count - mesh->num_vertices * 3; i++)
				aux_vert_pointer[i] = total_vert[i];
			for (int i = vert_count - mesh->num_vertices * 3, j = 0; i < vert_count; i++, j++)
				aux_vert_pointer[i] = mesh->vertices[j];

			delete[] total_vert;
			total_vert = aux_vert_pointer;
		}
		else {
			total_vert = new float[vert_count];

			for (int i = 0; i < vert_count; i++)
				total_vert[i] = mesh->vertices[i];
		}
	}

	Init(total_vert, vert_count);
}

void ComponentAABB::Draw() {

	glLineWidth(2.0f);

	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_LINES);

	glVertex3f(vertices[0], vertices[1], vertices[2]);
	glVertex3f(vertices[3], vertices[4], vertices[5]);

	glVertex3f(vertices[0], vertices[1], vertices[2]);
	glVertex3f(vertices[6], vertices[7], vertices[8]);

	glVertex3f(vertices[0], vertices[1], vertices[2]);
	glVertex3f(vertices[12], vertices[13], vertices[14]);

	glVertex3f(vertices[21], vertices[22], vertices[23]);
	glVertex3f(vertices[9], vertices[10], vertices[11]);

	glVertex3f(vertices[21], vertices[22], vertices[23]);
	glVertex3f(vertices[18], vertices[19], vertices[20]);

	glVertex3f(vertices[21], vertices[22], vertices[23]);
	glVertex3f(vertices[15], vertices[16], vertices[17]);

	glVertex3f(vertices[3], vertices[4], vertices[5]);
	glVertex3f(vertices[9], vertices[10], vertices[11]);

	glVertex3f(vertices[3], vertices[4], vertices[5]);
	glVertex3f(vertices[15], vertices[16], vertices[17]);
	
	glVertex3f(vertices[6], vertices[7], vertices[8]);
	glVertex3f(vertices[18], vertices[19], vertices[20]);

	glVertex3f(vertices[6], vertices[7], vertices[8]);
	glVertex3f(vertices[9], vertices[10], vertices[11]);

	glVertex3f(vertices[9], vertices[10], vertices[11]);
	glVertex3f(vertices[21], vertices[22], vertices[23]);

	glVertex3f(vertices[9], vertices[10], vertices[11]);
	glVertex3f(vertices[3], vertices[4], vertices[5]);

	glVertex3f(vertices[12], vertices[13], vertices[14]);
	glVertex3f(vertices[18], vertices[19], vertices[20]);

	glVertex3f(vertices[18], vertices[19], vertices[20]);
	glVertex3f(vertices[21], vertices[22], vertices[23]);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glLineWidth(1.0f);

}

float ComponentAABB::GetMinX() {

	float min_x = vertices[0];

	for (int i = 3; i < BB_VERTNUM; i += 3)
		min_x = (vertices[i] < min_x ? vertices[i] : min_x);

	return min_x;
}

float ComponentAABB::GetMaxX() {

	float max_x = vertices[0];

	for (int i = 3; i < BB_VERTNUM; i += 3)
		max_x = (vertices[i] > max_x ? vertices[i] : max_x);

	return max_x;
}

float ComponentAABB::GetMinY() {

	float min_y = vertices[1];

	for (int i = 4; i < BB_VERTNUM; i += 3)
		min_y = (vertices[i] < min_y ? vertices[i] : min_y);

	return min_y;
}

float ComponentAABB::GetMaxY() {

	float max_y = vertices[1];

	for (int i = 4; i < BB_VERTNUM; i += 3)
		max_y = (vertices[i] > max_y ? vertices[i] : max_y);

	return max_y;
}


float ComponentAABB::GetMinZ() {

	float min_z = vertices[2];

	for (int i = 5; i < BB_VERTNUM; i += 3)
		min_z = (vertices[i] < min_z ? vertices[i] : min_z);

	return min_z;
}

float ComponentAABB::GetMaxZ() {

	float max_z = vertices[2];

	for (int i = 5; i < BB_VERTNUM; i += 3)
		max_z = (vertices[i] > max_z ? vertices[i] : max_z);

	return max_z;
}

OBB::OBB(ComponentAABB& parent_aabb) {

	vertices = new float[BB_VERTNUM];

	float center_X = ((parent_aabb.GetMinX() + parent_aabb.GetMaxX()) / 2);
	float center_Y = ((parent_aabb.GetMinY() + parent_aabb.GetMaxY()) / 2);
	float center_Z = ((parent_aabb.GetMinZ() + parent_aabb.GetMaxZ()) / 2);

	for (int i = 0; i < BB_VERTNUM; i += 3) {
		vertices[i] = parent_aabb.vertices[i] - center_X;
		vertices[i + 1] = parent_aabb.vertices[i + 1] - center_Y;
		vertices[i + 2] = parent_aabb.vertices[i + 2] - center_Z;
	}
}