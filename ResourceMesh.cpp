#include "ResourceMesh.h"
#include "Color.h"

ResourceMesh::ResourceMesh(std::string uid) : Resource(uid, Res_Type::mesh)
{}

ResourceMesh::~ResourceMesh() 
{
	UnLoadToMemory();
	delete[] buffer;
}

void ResourceMesh::LoadToMemory()
{
	char* cursor = buffer;

	uint ranges[5];					 // amount of vertices / tris / normals / colors / texture_coords
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	num_vertices = ranges[0];
	num_tris = ranges[1];
	num_normals = ranges[2];
	num_colors = ranges[3];
	num_texcoords = ranges[4];

	// Load vertices
	cursor += bytes;
	bytes = sizeof(float3) * num_vertices;
	vertices = new float3[num_vertices];
	memcpy(vertices, cursor, bytes);

	// Load indices
	cursor += bytes;
	bytes = sizeof(Tri) * num_tris;
	tris = new Tri[num_tris];
	memcpy(tris, cursor, bytes);

	if (num_tris > 0)
	{
		glGenBuffers(1, (GLuint*) &(id_tris));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_tris);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Tri) * num_tris, tris, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Load normals
	cursor += bytes;
	bytes = sizeof(float3) * num_normals;
	normals = new float3[num_normals];
	memcpy(normals, cursor, bytes);

	if (num_normals > 0)
	{
		glGenBuffers(1, (GLuint*) &(id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * num_normals, normals, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Load colors
	cursor += bytes;
	bytes = sizeof(Color) * num_colors;
	colors = new Color[num_colors];
	memcpy(colors, cursor, bytes);

	if (num_colors > 0)
	{
		glGenBuffers(1, (GLuint*) &(id_colors));
		glBindBuffer(GL_ARRAY_BUFFER, id_colors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * num_colors, colors, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Load texcoords
	cursor += bytes;
	bytes = sizeof(float2) * num_texcoords;
	texcoords = new float2[num_texcoords];
	memcpy(texcoords, cursor, bytes);

	if (num_texcoords > 0)
	{
		glGenBuffers(1, (GLuint*) &(id_texcoords));
		glBindBuffer(GL_ARRAY_BUFFER, id_texcoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * num_texcoords, texcoords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}

void ResourceMesh::UnLoadToMemory()
{
	if (tris)		delete[] tris;
	if (vertices)	delete[] vertices;
	if (normals)	delete[] normals;
	if (colors)		delete[] colors;
	if (texcoords)	delete[] texcoords;

	if (num_tris) glDeleteBuffers(1, (GLuint*) &(id_tris));
	if (num_normals) glDeleteBuffers(1, (GLuint*) &(id_normals));
	if (id_colors) glDeleteBuffers(1, &id_colors);
	if (num_texcoords) glDeleteBuffers(1, (GLuint*) &(id_texcoords));
	
}
