#include "GameObject.h"
#include "ComponentMesh.h"


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
	
	ComponentMesh* mesh = (ComponentMesh*)FindComponent(COMPONENT_MESH);   //should draw all meshes
	
	if(mesh)
		mesh->Draw();

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		(*it)->DrawRecursive();
}