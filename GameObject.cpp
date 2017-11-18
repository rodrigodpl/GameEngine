#include "GameObject.h"
#include "ComponentMesh.h"
#include "imgui.h"
#include "ComponentCamera.h"
#include "ComponentAABB.h"
#include "ModuleJSON.h"
#include "MD5.h"

GameObject::GameObject(const char* obj_name, GameObject* parent) : name(obj_name), parent(parent), uid(CreateUID(name.c_str(), name.length()))
{}

GameObject::GameObject(GameObject& game_obj) 
{
	parent = game_obj.parent;
	name = game_obj.name + "_copy";
	uid = CreateUID(name.c_str(), name.length());

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) 
		components.push_back((*it)->Duplicate());

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		children.push_back(new GameObject(*(*it)));
}

GameObject::GameObject() 
{}

GameObject::~GameObject() 
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it)->type != COMPONENT_MATERIAL)    //  materials are stored and deleted by scene_intro
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

	float3 min_point = comp_aabb->GetMinP(); 
	float3 max_point = comp_aabb->GetMaxP();

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

			for (int i = 0; i < mesh->num_tris; i += 3) {

				float3 vert1 = mesh->vertices[mesh->tris[i].vert1];
				float3 vert2 = mesh->vertices[mesh->tris[i].vert2];
				float3 vert3 = mesh->vertices[mesh->tris[i].vert3];

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


void GameObject::HierarchyTree(std::string& selected_obj_uid)
{
	int flags = 0;
	if (children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;
	if (ImGui::IsItemClicked()) selected_obj_uid = uid;
	if (selected_obj_uid == uid) flags |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx(name.c_str(), flags)) {
		for (int aux = 0; aux < children.size(); aux++)	children[aux]->HierarchyTree(selected_obj_uid);

		ImGui::TreePop();
	}
}

void GameObject::Save(JSON_file& save_file, uint& obj_index)
{
	std::string base_name = ("objects.");
	base_name.append(std::to_string(obj_index));

	save_file.WriteString(std::string(".name").insert(0, base_name).c_str(), name.c_str());
	save_file.WriteString(std::string(".uid").insert(0, base_name).c_str(), uid.c_str());
	save_file.WriteBool(std::string(".enabled").insert(0, base_name).c_str(), enabled);

	if(parent)
		save_file.WriteString(std::string(".parent_uid").insert(0, base_name).c_str(), parent->uid.c_str());
	else
		save_file.WriteString(std::string(".parent_uid").insert(0, base_name).c_str(), "none");

	save_file.WriteNumber(std::string(".number of components").insert(0, base_name).c_str(), components.size());
	uint comp_index = 0;
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++, comp_index++)
	{
		std::string component_code(base_name);
		component_code.append(".components."); component_code.append(std::to_string(comp_index));
		(*it)->Save(save_file, component_code.c_str());
	}

	for (std::vector<GameObject*>::iterator it2 = children.begin(); it2 != children.end(); it2++)
		(*it2)->Save(save_file, ++obj_index);
}

void GameObject::Load(JSON_file& save_file, uint& obj_index)
{
	std::string base_name = ("objects.");
	base_name.append(std::to_string(obj_index));

	name = save_file.ReadString(std::string(".name").insert(0, base_name).c_str());
	uid = save_file.ReadString(std::string(".uid").insert(0, base_name).c_str());
	enabled = save_file.ReadBool(std::string(".enabled").insert(0, base_name).c_str());

	parent_uid = save_file.ReadString(std::string(".parent_uid").insert(0, base_name).c_str());
	// parent pointer MUST be setup by scene loading method AFTER game object loading

	uint comp_num = save_file.ReadNumber(std::string(".number of components").insert(0, base_name).c_str());

	for (uint comp_index = 0; comp_index < comp_num; comp_index++)
	{
		std::string component_code(base_name);
		component_code.append(".components."); component_code.append(std::to_string(comp_index));

		Component_type type = (Component_type)((uint)save_file.ReadNumber(std::string(".type").insert(0, component_code).c_str()));
		Component* new_comp = nullptr;

		switch (type)
		{
		case COMPONENT_MESH:		new_comp = (Component*) new ComponentMesh();		break;
		case COMPONENT_TRANSFORM:	new_comp = (Component*) new ComponentTransform();	break;
		case COMPONENT_CAMERA:		new_comp = (Component*) new ComponentCamera();		break;
		case COMPONENT_AABB:		new_comp = (Component*) new ComponentAABB();		break;
		}

		if (new_comp)
		{
			new_comp->Load(save_file, component_code.c_str());
			components.push_back(new_comp);
		}
	}

	for (std::vector<GameObject*>::iterator it2 = children.begin(); it2 != children.end(); it2++)
		(*it2)->Load(save_file, ++obj_index);

}

void GameObject::FindParent(std::vector<GameObject*>& objects)
{
	if (parent_uid != "none")
	{
		for (std::vector<GameObject*>::iterator it = objects.begin(); it != objects.end() && parent == nullptr; it++)
			parent = (*it)->RetrieveParent(parent_uid);
	}

	for (std::vector<GameObject*>::iterator it2 = children.begin(); it2 != children.end(); it2++)
		(*it2)->FindParent(objects);
}

GameObject* GameObject::RetrieveParent(std::string& parent_uid)
{
	if (uid == parent_uid) return this;
	else
	{
		GameObject* searched_parent = nullptr;
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		{
			if(searched_parent = (*it)->RetrieveParent(parent_uid)) return searched_parent;
		}
		
	}
	return nullptr;
}
