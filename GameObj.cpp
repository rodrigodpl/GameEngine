#include "GameObj.h"

GameObject::GameObject() {



}

GameObject::GameObject(GameObject& game_obj) {

	parent = game_obj.parent;
	name = game_obj.name + "_copy";

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) 
		components.push_back((*it)->Duplicate());
	

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		children.push_back(new GameObject(*(*it)));
}

GameObject::~GameObject() {

	components.clear();
	children.clear();
}

Component* GameObject::FindComponent(Component_type type) {

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it)->type == type)
			return *it;
	}
}

void GameObject::Update(float dt) {

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it)->enabled)
			(*it)->Update(dt);
	}
	
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
		(*it)->Update(dt);
}