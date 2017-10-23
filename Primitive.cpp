
#include "Globals.h"
#include "Primitive.h"
#include "glut/glut.h"
#include "ModuleSceneIntro.h"

#pragma comment (lib, "glut/glut32.lib")

#include "glew-2.1.0\include\GL\glew.h"

#include <gl/GL.h>
#include <gl/GLU.h>

// ------------------------------------------------------------
Primitive::Primitive() : color(White), wire(false), axis(false), alpha(1.0f), primitive_type(PrimitiveTypes::Primitive_Point)
{
	color = { (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100 };
}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetPrimitiveType() const
{
	return primitive_type;
}

// ------------------------------------------------------------
void Primitive::Draw() 
{

	if (axis == true)
		DrawAxis();

	glColor4f(color.r, color.g, color.b, alpha);

	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

void Primitive::BuildVert() {}

void Primitive::DrawAxis() {

	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	glEnd();

	glLineWidth(1.0f);

}

// CUBE ============================================
Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	primitive_type = PrimitiveTypes::Primitive_Cube;
	BuildVert();
	LoadDataToVRAM();
}

Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	primitive_type = PrimitiveTypes::Primitive_Cube;
	BuildVert();
	LoadDataToVRAM();
	//ModuleGui::Get_colour(color);
}

void Cube::BuildVert() {

	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	num_vertices = 8;
	float* v = vertices;

	*v++ = -sx; *v++ = -sy; *v++ = -sz;   *v++ = -sx; *v++ = sy; *v++ = -sz;  
	*v++ =  sx; *v++ = -sy; *v++ = -sz;   *v++ =  sx; *v++ = sy; *v++ = -sz; 
	*v++ = -sx; *v++ = -sy; *v++ = sz ;   *v++ = -sx; *v++ = sy; *v++ = sz ; 
	*v++ =  sx; *v++ = -sy; *v++ = sz ;   *v++ =  sx; *v++ = sy; *v++ = sz ; 

	num_indices = 36;
	uint* i = indices;
	
	*i++ = 0; *i++ = 1; *i++ = 2;      *i++ = 3; *i++ = 2; *i++ = 1;      //front
	*i++ = 6; *i++ = 5; *i++ = 4;      *i++ = 5; *i++ = 6; *i++ = 7;      //back
	*i++ = 5; *i++ = 3; *i++ = 1;      *i++ = 3; *i++ = 5; *i++ = 7;      //up
	*i++ = 0; *i++ = 2; *i++ = 4;      *i++ = 6; *i++ = 4; *i++ = 2;      //down	
	*i++ = 4; *i++ = 1; *i++ = 0;      *i++ = 1; *i++ = 4; *i++ = 5;      //left	
	*i++ = 2; *i++ = 3; *i++ = 6;      *i++ = 7; *i++ = 6; *i++ = 3;      //right

}

void Cube::InnerRender() const
{	
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//
// SPHERE ============================================
//Sphere::Sphere() : Primitive(), radius(1.0f)
//{
//	type = PrimitiveTypes::Primitive_Sphere;
//	BuildVert();
//	color = { (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100 };
//}
//
//Sphere::Sphere(float radius, uint rings, uint sectors) : Primitive(), radius(radius), rings(rings), sectors(sectors)
//{
//	type = PrimitiveTypes::Primitive_Sphere;
//	BuildVert();
//	color = { (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100 };
//}
//
//void Sphere::BuildVert() {
//
//	float const R = 1. / (float)(rings - 1);
//	float const S = 1. / (float)(sectors - 1);
//	int r, s;
//
//	vertices.resize(rings * sectors * 3);
//	normals.resize(rings * sectors * 3);
//	texcoords.resize(rings * sectors * 2);
//
//	std::vector<Vertex>::iterator v = vertices.begin();
//	std::vector<GLfloat>::iterator n = normals.begin();
//	std::vector<GLfloat>::iterator t = texcoords.begin();
//
//	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
//
//		float const y = sin(-M_PI_2 + M_PI * r * R);
//		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
//		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
//
//		*t++ = s*S;
//		*t++ = r*R;
//
//		*v++ = { x * radius,y * radius, z * radius };
//
//		*n++ = x; *n++ = y; *n++ = z;
//	}
//
//	indices.resize(rings * sectors * 4);
//	std::vector<GLushort>::iterator i = indices.begin();
//	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
//		*i++ = r * sectors + s;
//		*i++ = r * sectors + (s + 1);
//		*i++ = (r + 1) * sectors + (s + 1);
//		*i++ = (r + 1) * sectors + s;
//	}
//
//	BindBuffer();
//
//}
//
//void Sphere::InnerRender() const
//{
//	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buff_id);
//
//	glColor4f(color.r, color.g, color.b, alpha);
//
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_NORMAL_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//	glVertexPointer(3, GL_FLOAT, 0, NULL);
//	glNormalPointer(GL_FLOAT, 0, &normals[0]);
//	glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
//	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, NULL);
//
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	glDisableClientState(GL_NORMAL_ARRAY);
//	glDisableClientState(GL_VERTEX_ARRAY);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//}

// CYLINDER ============================================
Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	primitive_type = PrimitiveTypes::Primitive_Cylinder;
	BuildVert();
	LoadDataToVRAM();
}

Cylinder::Cylinder(float radius, float height, uint sides) : Primitive(), radius(radius), height(height), sides(sides)
{
	primitive_type = PrimitiveTypes::Primitive_Cylinder;
	BuildVert();
	LoadDataToVRAM();
}

void Cylinder::BuildVert() {

	num_vertices = ((360 / (360 / sides)) * 2) + 4;    //sides  (vertex per circumference) * 2 (circumferences) + 2 (circumference center)
	float* v = vertices;

	*v++ =  height*0.5f ; *v++ =   0;  *v++ =  0;     //vertices[0] is top center vertex
	*v++ =  -height*0.5f; *v++ =   0;  *v++ =  0;    //vertices[1] is top bottom vertex

	for (int a = 360; a >= 0; a -= (360 / sides))
	{
		float b = a * M_PI / 180; // degrees to radians
		*v++ = radius * cos(b); *v++ = height * 0.5f; *v++ = radius * sin(b);    //vertices[2] is first vertex in top circumference
		*v++ = radius * cos(b); *v++ = -height*0.5f;  *v++ = radius * sin(b);    //vertices[3] is first vertex in bottom circumference
	} 

	num_indices = (num_vertices * 6) - 4;
	uint* i = indices;

	for (int a = 2; (a + 2) < num_vertices; a++)
	{
		*i++ = a + ((a + 1) % 2 ? 2 : 0); *i++ = a % 2; *i++ = a + (a % 2 ? 2 : 0);	// top and bottom circ
		*i++ = a + ((a + 1) % 2 ? 0 : 2); *i++ = a + 1; *i++ = a + (a % 2 ? 0 : 2);   // side
	}

}

void Cylinder::InnerRender() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

// LINE ==================================================
Line::Line() : Primitive(), origin({ 0, 0, 0 }), destination({ 1, 1, 1 })
{
	primitive_type = PrimitiveTypes::Primitive_Line;
}

Line::Line(Vertex origin, Vertex dest) : Primitive(), origin(origin), destination(dest)
{
	primitive_type = PrimitiveTypes::Primitive_Line;
}


void Line::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
Plane::Plane() : Primitive(), normal(0, 1, 0), constant(1)
{
	primitive_type = PrimitiveTypes::Primitive_Plane;
}

Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	primitive_type = PrimitiveTypes::Primitive_Plane;
}

void Plane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 20.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}