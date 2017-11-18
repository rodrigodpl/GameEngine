
#include "Globals.h"
#include "Primitive.h"
#include "glut/glut.h"
#include "ModuleSceneIntro.h"

#pragma comment (lib, "glut/glut32.lib")

#include "glew-2.1.0\include\GL\glew.h"
#include "ComponentTransform.h"

#include <gl/GL.h>
#include <gl/GLU.h>

// ------------------------------------------------------------
Primitive::Primitive() : color(White), wire(false), axis(false), alpha(1.0f), primitive_type(PrimitiveTypes::Primitive_Point)
{
	color = { (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100 };
	transform = new ComponentTransform();
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
	vertices = new float3[num_vertices];

	vertices[0].x = -sx; vertices[0].y = -sy; vertices[0].z = -sz;
	vertices[1].x = sx;  vertices[1].y = -sy; vertices[1].z = -sz;
	vertices[2].x = -sx; vertices[2].y = -sy; vertices[2].z = sz;
	vertices[3].x = sx;  vertices[3].y = -sy; vertices[3].z = sz;

	vertices[4].x = -sx; vertices[4].y = sy;  vertices[4].z = -sz;
	vertices[5].x = sx;  vertices[5].y = sy;  vertices[5].z = -sz;
	vertices[6].x = -sx; vertices[6].y = sy;  vertices[6].z = sz;
	vertices[7].x = sx;  vertices[7].y = sy;  vertices[7].z = sz;

	num_tris = 12;
	tris = new Tri[num_tris];

	tris[0].vert1 = 0; tris[0].vert2 = 1; tris[0].vert3 = 2;    tris[1].vert1 = 3; tris[1].vert2 = 2; tris[1].vert3 = 1;      //front
	tris[2].vert1 = 6; tris[2].vert2 = 5; tris[2].vert3 = 4;    tris[3].vert1 = 5; tris[3].vert2 = 6; tris[3].vert3 = 7;      //back
	tris[4].vert1 = 5; tris[4].vert2 = 3; tris[4].vert3 = 1;    tris[5].vert1 = 3; tris[5].vert2 = 5; tris[5].vert3 = 7;      //up
	tris[6].vert1 = 0; tris[6].vert2 = 2; tris[6].vert3 = 4;    tris[7].vert1 = 6; tris[7].vert2 = 4; tris[7].vert3 = 2;      //down	
	tris[8].vert1 = 4; tris[8].vert2 = 1; tris[8].vert3 = 0;    tris[9].vert1 = 1; tris[9].vert2 = 4; tris[9].vert3 = 5;      //left	
	tris[10].vert1 = 2; tris[10].vert2 = 3; tris[10].vert3 = 6; tris[11].vert1 = 7; tris[11].vert2 = 6; tris[11].vert3 = 3;   //right

}

void Cube::InnerRender() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_tris);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, num_tris * 3, GL_UNSIGNED_INT, NULL);
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
Cyl::Cyl() : Primitive(), radius(1.0f), height(1.0f)
{
	primitive_type = PrimitiveTypes::Primitive_Cylinder;
	BuildVert();
	LoadDataToVRAM();
}

Cyl::Cyl(float radius, float height, uint sides) : Primitive(), radius(radius), height(height), sides(sides)
{
	primitive_type = PrimitiveTypes::Primitive_Cylinder;
	BuildVert();
	LoadDataToVRAM();
}

void Cyl::BuildVert() {

	num_vertices = ((360 / (360 / sides)) * 2) + 4;    //sides  (vertex per circumference) * 2 (circumferences) + 2 (circumference center)
	vertices = new float3[num_vertices];

	vertices[0].x = height*0.5f;  vertices[0].y = 0; vertices[0].z = 0;     //vertices[0] is top center vertex
	vertices[1].x = -height*0.5f; vertices[1].y = 0; vertices[2].z = 0;    //vertices[1] is top bottom vertex

	for (int a = 360, i = 2; a >= 0; a -= (360 / sides), i++)
	{
		float b = a * M_PI / 180; // degrees to radians
		vertices[i].x = radius * cos(b); vertices[i].y = height * 0.5f; vertices[i].z = radius * sin(b);    //vertices[2] is first vertex in top circumference
		i++;
		vertices[i].x = radius * cos(b); vertices[i].y = -height*0.5f;  vertices[i].x = radius * sin(b);    //vertices[3] is first vertex in bottom circumference
	}

	num_tris = (num_vertices * 2) - 4;
	tris = new Tri[num_tris];

	for (int a = 2, i = 0; (a + 2) < num_vertices; a++, i++)
	{
		tris[i].vert1 = a + ((a + 1) % 2 ? 2 : 0); tris[i].vert2 = a % 2; tris[i].vert3 = a + (a % 2 ? 2 : 0);	// top and bottom circ
		i++;
		tris[i].vert1 = a + ((a + 1) % 2 ? 0 : 2); tris[i].vert2 = a + 1; tris[i].vert3 = a + (a % 2 ? 0 : 2);   // side
	}

}

void Cyl::InnerRender() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_tris);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, num_tris * 3, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

// LINE ==================================================
LineSegm::LineSegm() : Primitive(), origin({ 0, 0, 0 }), destination({ 1, 1, 1 })
{
	primitive_type = PrimitiveTypes::Primitive_Line;
}

LineSegm::LineSegm(Vertex origin, Vertex dest) : Primitive(), origin(origin), destination(dest)
{
	primitive_type = PrimitiveTypes::Primitive_Line;
}


void LineSegm::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
PrimPlane::PrimPlane() : Primitive(), normal(0, 1, 0), constant(1)
{
	primitive_type = PrimitiveTypes::Primitive_Plane;
}

PrimPlane::PrimPlane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	primitive_type = PrimitiveTypes::Primitive_Plane;
}

void PrimPlane::InnerRender() const
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