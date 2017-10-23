
#pragma once
#include "glmath.h"
#include "Color.h"
#include "glew-2.1.0\include\GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ModuleGui.h"

#include "ComponentMesh.h"

#include <vector>


enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

struct Vertex {
	GLfloat x, y, z;
};


class Primitive : public ComponentMesh
{
public:

	Primitive();

	virtual void	Draw();
	virtual void	InnerRender() const;
	virtual void    BuildVert();
	void			DrawAxis();
	PrimitiveTypes	GetPrimitiveType() const;

public:
	
	Color	color;
	float	alpha;
	bool	axis, wire;

	PrimitiveTypes primitive_type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
	void BuildVert();
public:
	vec3 size;
};

// ============================================
//class Sphere : public Primitive
//{
//public:
//	Sphere();
//	Sphere(float radius, uint rings, uint sectors);
//	void InnerRender() const;
//	void BuildVert();
//public:
//	float radius;
//	uint rings, sectors;
//};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height, uint sides);
	void InnerRender() const;
	void BuildVert();
public:
	float radius;
	float height;
	uint sides;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(Vertex origin, Vertex dest);
	void InnerRender() const;
public:
	Vertex origin;
	Vertex destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane();
	Plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};