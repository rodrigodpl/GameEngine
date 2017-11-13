#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentAABB.h"

GameObject::GameObject(const char* obj_name, GameObject* parent) : name(obj_name), parent(parent)
{}

GameObject::GameObject(GameObject& game_obj) {

	parent = game_obj.parent;
	name = game_obj.name + "_copy";

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) 
		components.push_back((*it)->Duplicate());
	

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		children.push_back(new GameObject(*(*it)));
}

GameObject::~GameObject() {

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it)->type == COMPONENT_MATERIAL)    //  materials are stored and deleted by scene_intro
			delete *it;
	}

	children.clear();
}

Component* GameObject::FindComponent(Component_type type) {

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it)->type == type)
			return *it;
	}

	return nullptr;
}

std::vector<Component*> GameObject::FindComponents(Component_type type) {

	std::vector<Component*> ret;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it)->type == type)
			ret.push_back(*it);
	}

	return ret;
}

GameObject* GameObject::FindChild(const char* obj_name) {

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
		if ((*it)->name == obj_name)
			return *it;
	}
	
	return nullptr;
}

void GameObject::UpdateRecursive(float dt) {

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it)->enabled)
			(*it)->Update(dt);
	}
	
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		(*it)->UpdateRecursive(dt);
}


void GameObject::DrawRecursive() {

	if (!culled) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
			(*it)->Draw();
	}

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		(*it)->DrawRecursive();
}

void GameObject::CullRecursive(ComponentCamera* cam) 
{
	ComponentAABB* aabb = (ComponentAABB*)FindComponent(COMPONENT_AABB);

	if (!aabb) {
		components.push_back(new ComponentAABB(*this));   // safeguard in case the gameobject has no AABB
		aabb = (ComponentAABB*)components.back();
	}

	culled = cam->FrustumCulling(*aabb);

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		(*it)->CullRecursive(cam);
}


void GameObject::RayCastAgainstAABBs(math::Ray ray, std::list<RayHit>& outHits) {

	ComponentAABB* comp_aabb = (ComponentAABB*)FindComponent(COMPONENT_AABB);

	if (!comp_aabb) {
		components.push_back(new ComponentAABB(*this));   // safeguard in case the gameobject has no AABB
		comp_aabb = (ComponentAABB*)components.back();
	}

	float3 min_point = { comp_aabb->GetMinX(), comp_aabb->GetMinY(), comp_aabb->GetMinZ() };
	float3 max_point = { comp_aabb->GetMaxX(), comp_aabb->GetMaxY(), comp_aabb->GetMaxZ() };

	AABB aabb(min_point, max_point);

	float near_d = 0; float far_d = 0;

	if (ray.Intersects(aabb, near_d, far_d)) {

		RayHit hit;
		hit.hit_distance = near_d; hit.object = this;
		outHits.push_back(hit);
	}

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		(*it)->RayCastAgainstAABBs(ray, outHits);

}


void GameObject::RayCastAgainstMeshes(Ray ray, std::list<RayHit>& outHits) {

	std::vector<Component*> meshes = FindComponents(COMPONENT_MESH);

	if (meshes.empty())
		return;
	else
	{
		for (std::vector<Component*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
			
			ComponentMesh* mesh = (ComponentMesh*)(*it);

			for (int i = 0; i < mesh->num_indices; i += 3) {

				float3 vert1, vert2, vert3;
				vert1 = { mesh->vertices[mesh->indices[i]], mesh->vertices[mesh->indices[i] + 1], mesh->vertices[mesh->indices[i + 2] + 2] };
				vert2 = { mesh->vertices[mesh->indices[i + 1]], mesh->vertices[mesh->indices[i + 1] + 1], mesh->vertices[mesh->indices[i + 1] + 2] };
				vert3 = { mesh->vertices[mesh->indices[i + 2]], mesh->vertices[mesh->indices[i + 2] + 1], mesh->vertices[mesh->indices[i + 2] +2] };

				Triangle tri(vert1, vert2, vert3);

				if (ray.Intersects(tri)) {

					RayHit hit;
					hit.object = this;
					hit.hit_distance = tri.CenterPoint().Distance(ray.pos);
					outHits.push_back(hit);
				}
			}
		}
	}

}

/*void GameObject::CreateTree() {
	for (int aux = 0; aux < children.size(); aux++) {
		children[aux]->CreateTree();
		
	}
}*/