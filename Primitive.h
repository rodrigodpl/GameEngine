
#pragma once
#include "glmath.h"
#include "Color.h"
#include "glew-2.1.0\include\GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ModuleGui.h"

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


class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	virtual void    BuildVert();
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	void			BindBuffer();
	PrimitiveTypes	GetType() const;

public:
	
	Color	color;
	float	alpha;
	mat4x4	transform;
	bool	axis,wire, visible;

	std::vector<Vertex>		vertices;	uint vert_buff_id;
	std::vector<GLfloat>	normals;	uint normal_buff_id;
	std::vector<GLfloat>	texcoords;	uint texcoord_buff_id;
	std::vector<GLushort>	indices;	uint index_buff_id;

protected:
	PrimitiveTypes type;
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
class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(float radius, uint rings, uint sectors);
	void InnerRender() const;
	void BuildVert();
public:
	float radius;
	uint rings, sectors;
};

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