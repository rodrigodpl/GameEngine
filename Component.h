#pragma once


enum Component_type {

	COMPONENT_NONE = 0,
	COMPONENT_PHYSICS,
	COMPONENT_INPUT,
	COMPONENT_RENDERER,

};


class Component {

public: 

	Component(Component_type type = COMPONENT_NONE) : type(type), enabled(true) {};
	virtual ~Component() {};

	virtual Component* Duplicate() { return new Component(COMPONENT_NONE); };
	virtual void Update(float dt) {};
	virtual void Enable() { enabled = true; };
	virtual void Disable() { enabled = false; };

public:
	Component_type type;
	bool enabled;
};