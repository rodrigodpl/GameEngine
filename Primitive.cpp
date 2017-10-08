
#include "Globals.h"
#include "glew-2.1.0\include\GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#include "glut/glut.h"
#include "ModuleSceneIntro.h"

#pragma comment (lib, "glut/glut32.lib")

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), visible(true), alpha(1.0f), type(PrimitiveTypes::Primitive_Point)
{
	
}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if(axis == true)
	{
		// Draw Axis Grid
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

	glColor3f(color.r, color.g, color.b);

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

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

void Primitive::BuildVert() {}

void Primitive::BindBuffer() {

	glGenBuffers(1, (GLuint*) &(vert_buff_id));
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(index_buff_id));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buff_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

// CUBE ============================================
Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
	BuildVert();
	color = { (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100 };
}

Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
	BuildVert();
	color = { (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100 };
}

void Cube::BuildVert() {

	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	vertices.resize(8);
	std::vector<Vertex>::iterator v = vertices.begin();

	*v++ = { -sx, -sy, -sz }; *v++ = { -sx,  sy, -sz }; 
	*v++ = {  sx, -sy, -sz }; *v++ = {  sx,  sy, -sz };
	*v++ = { -sx, -sy, sz  }; *v++ = { -sx,  sy, sz  };
	*v++ = {  sx, -sy, sz  }; *v++ = {  sx,  sy, sz  };

	indices.resize(36);
	std::vector<GLushort>::iterator i = indices.begin();
	
	*i++ = 0; *i++ = 1; *i++ = 2;      *i++ = 3; *i++ = 2; *i++ = 1;      //front
	*i++ = 6; *i++ = 5; *i++ = 4;      *i++ = 5; *i++ = 6; *i++ = 7;      //back
	*i++ = 5; *i++ = 3; *i++ = 1;      *i++ = 3; *i++ = 5; *i++ = 7;      //up
	*i++ = 0; *i++ = 2; *i++ = 4;      *i++ = 6; *i++ = 4; *i++ = 2;      //down	
	*i++ = 4; *i++ = 1; *i++ = 0;      *i++ = 1; *i++ = 4; *i++ = 5;      //left	
	*i++ = 2; *i++ = 3; *i++ = 6;      *i++ = 7; *i++ = 6; *i++ = 3;      //right

	BindBuffer();
}

void Cube::InnerRender() const
{	
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buff_id);

	glColor4f(color.r, color.g, color.b, alpha);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glNormalPointer(GL_FLOAT, 0, &normals[0]);
	//glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, NULL);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// SPHERE ============================================
Sphere::Sphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
	BuildVert();
	color = { (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100 };
}

Sphere::Sphere(float radius, uint rings, uint sectors) : Primitive(), radius(radius), rings(rings), sectors(sectors)
{
	type = PrimitiveTypes::Primitive_Sphere;
	BuildVert();
	color = { (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100 };
}

void Sphere::BuildVert() {

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texcoords.resize(rings * sectors * 2);

	std::vector<Vertex>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	std::vector<GLfloat>::iterator t = texcoords.begin();

	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {

		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*t++ = s*S;
		*t++ = r*R;

		*v++ = { x * radius,y * radius, z * radius };

		*n++ = x; *n++ = y; *n++ = z;
	}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}

	BindBuffer();

}

void Sphere::InnerRender() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buff_id);

	glColor4f(color.r, color.g, color.b, alpha);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, NULL);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// CYLINDER ============================================
Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

Cylinder::Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void Cylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();
}

// LINE ==================================================
Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
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
	type = PrimitiveTypes::Primitive_Plane;
}

Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void Plane::InnerRender() const
{
	glLineWidth(1.0f);

	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
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