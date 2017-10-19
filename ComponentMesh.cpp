#include "ComponentMesh.h"

#include "glew-2.1.0\include\GL\glew.h"

ComponentMesh::ComponentMesh() {}

ComponentMesh::ComponentMesh(aiMesh& mesh) {

	type = Component_type::COMPONENT_MESH;

	LoadDataFromAssimp(mesh);
	LoadDataToVRAM();
	
}

ComponentMesh::ComponentMesh(ComponentMesh& mesh) {

	type = Component_type::COMPONENT_MESH;

	num_indices = mesh.num_indices;		indices = mesh.indices;    // should be (new) + memcpy()
	num_vertices = mesh.num_vertices;	vertices = mesh.vertices;
	num_normals = mesh.num_normals;		normals = mesh.normals;
	num_colors = mesh.num_colors;		colors = mesh.colors;
	num_texcoords = mesh.num_texcoords;	texcoords = mesh.texcoords;

	mat = (ComponentMaterial*) mesh.mat->Duplicate();

	LoadDataToVRAM();

}

ComponentMesh::~ComponentMesh() {

	if(indices)
		delete indices;
	if (vertices)
		delete vertices;
	if (normals)
		delete normals;
	if (colors)
		delete colors;
	if (texcoords)
		delete texcoords;

}

Component* ComponentMesh::Duplicate()
{
	return (Component*) new ComponentMesh(*(this));
}

bool ComponentMesh::LoadDataFromAssimp(aiMesh& imp_mesh) {

	if (imp_mesh.HasFaces())
	{
		num_indices = imp_mesh.mNumFaces * 3;
		indices = new uint[num_indices]; // assume each face is a triangle
		for (uint i = 0; i < imp_mesh.mNumFaces; ++i)
		{
			if (imp_mesh.mFaces[i].mNumIndices != 3)
				return false;
			else
				memcpy(&indices[i * 3], imp_mesh.mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	if (imp_mesh.HasPositions()) {

		num_vertices = imp_mesh.mNumVertices;
		vertices = new float[num_vertices * 3];

		memcpy(vertices, imp_mesh.mVertices, sizeof(float) * num_vertices * 3);
	}

	if (imp_mesh.HasNormals()) {

		num_normals = num_vertices;
		normals = new float[num_normals * 3];

		memcpy(normals, imp_mesh.mNormals, sizeof(float) * num_normals * 3);
	}

	if (imp_mesh.HasTextureCoords(0)) {

		num_texcoords = num_vertices;
		texcoords = new float[num_texcoords * 3];

		memcpy(texcoords, imp_mesh.mTextureCoords[0], sizeof(float) * num_texcoords * 3);
	}

	return true;
}


void ComponentMesh::LoadDataToVRAM() {

	if (num_indices > 0)
	{
		glGenBuffers(1, (GLuint*) &(id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	if (num_vertices > 0) {

		glGenBuffers(1, (GLuint*) &(id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	if (num_normals > 0) {

		glGenBuffers(1, (GLuint*) &(id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_normals * 3, normals, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	if (num_texcoords > 0) {
		glGenBuffers(1, (GLuint*) &(id_texcoords));
		glBindBuffer(GL_ARRAY_BUFFER, id_texcoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_texcoords * 3, texcoords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}


void ComponentMesh::Draw() {

	if(mat)
		glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, id_texcoords);
	glBindBuffer(GL_NORMAL_ARRAY, id_normals);

	glColor3f(1.0f, 1.0f, 1.0f);

	if (mat)
		glBindTexture(GL_TEXTURE_2D, mat->textures.back()->gl_binding);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	glNormalPointer(GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	if (mat)
		glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_NORMAL_ARRAY, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	if (mat)
		glDisable(GL_TEXTURE_2D);

}