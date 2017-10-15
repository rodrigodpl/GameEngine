#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "ModuleImporter.h"
#include <list>

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

	void Draw();
	void AddCube(float size_x, float size_y, float size_z);
	void AddSphere(float radius, uint rings = 25, uint sectors = 25);
	void AddCylinder(float radius, float height, uint sides = 30);
	void AddLine(Vertex origin, Vertex dest);

	void ShowPrimitives(PrimitiveTypes type);
	void ShowNextMesh();

	std::list<Primitive*> primitives;
	std::list<Mesh> meshes;

	uint current_mesh;

};
