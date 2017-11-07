#pragma once


enum Component_type {

	COMPONENT_NONE = 0,
	COMPONENT_MATERIAL,
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_AABB

};


class Component {

public: 

	Component(){};
	virtual ~Component() {};

	virtual Component* Duplicate() { return new Component(); };
	virtual void Update(float dt) {};
	virtual void Enable() { enabled = true; };
	virtual void Disable() { enabled = false; };

public:
	Component_type type = COMPONENT_NONE;
	bool enabled = true;
};