#pragma once
#include "ModuleJSON.h"

enum Component_type {

	COMPONENT_NONE = 0,
	COMPONENT_MATERIAL,
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_AABB,
	COMPONENT_CAMERA

};


class Component {

public: 

	Component(){};
	virtual ~Component() {};

	virtual Component* Duplicate() { return new Component(); };
	virtual void Update(float dt) {};
	virtual void Draw() {};

	virtual void Save(JSON_file& save_file, const char* component_code) {};
	virtual void Load(JSON_file& save_file, const char* component_code) {};

	void Enable() { enabled = true; };
	void Disable() { enabled = false; };

public:
	Component_type type = COMPONENT_NONE;
	bool enabled = true;
};