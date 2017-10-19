#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "ComponentMesh.h"
#include "GameObject.h"

#include <list>
#include <vector>

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void DrawScene();

public:

	std::list<GameObject*> game_objects;

	std::vector<ComponentMaterial*> materials;

};
