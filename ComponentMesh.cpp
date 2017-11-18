#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "glew-2.1.0\include\GL\glew.h"

ComponentMesh::ComponentMesh() 
{ 
	type = Component_type::COMPONENT_MESH; 
}

ComponentMesh::ComponentMesh(aiMesh& mesh) {

	type = Component_type::COMPONENT_MESH;

	transform = new ComponentTransform();

	LoadDataFromAssimp(mesh);
	LoadDataToVRAM();
	
}

ComponentMesh::ComponentMesh(ComponentMesh& mesh) {

	type = Component_type::COMPONENT_MESH;

	num_tris = mesh.num_tris;			tris = mesh.tris;    // should be (new) + memcpy()
	num_vertices = mesh.num_vertices;	vertices = mesh.vertices;
	num_normals = mesh.num_normals;		normals = mesh.normals;
	num_colors = mesh.num_colors;		colors = mesh.colors;
	num_texcoords = mesh.num_texcoords;	texcoords = mesh.texcoords;

	mat = (ComponentMaterial*) mesh.mat->Duplicate();
	transform = (ComponentTransform*) mesh.transform->Duplicate();;

	LoadDataToVRAM();

}

ComponentMesh::~ComponentMesh() {

	if (tris)		delete tris;
	if (vertices)	delete vertices;
	if (normals)	delete normals;
	if (colors)		delete colors;
	if (texcoords)	delete texcoords;

}

Component* ComponentMesh::Duplicate()
{
	return (Component*) new ComponentMesh(*(this));
}

bool ComponentMesh::LoadDataFromAssimp(aiMesh& imp_mesh) {

	if (imp_mesh.HasFaces())
	{
		num_tris = imp_mesh.mNumFaces;
		tris = new Tri[num_tris]; // assume each face is a triangle
		for (uint i = 0; i < imp_mesh.mNumFaces; ++i)
		{
			if (imp_mesh.mFaces[i].mNumIndices != 3)
				return false;
			else
				memcpy(&tris[i], imp_mesh.mFaces[i].mIndices, sizeof(Tri));
		}
	}

	if (imp_mesh.HasPositions()) {

		num_vertices = imp_mesh.mNumVertices;
		vertices = new float3[num_vertices];

		memcpy(vertices, imp_mesh.mVertices, sizeof(float3) * num_vertices);
	}

	if (imp_mesh.HasNormals()) {

		num_normals = num_vertices;
		normals = new float3[num_normals];

		memcpy(normals, imp_mesh.mNormals, sizeof(float3) * num_normals);
	}

	if (imp_mesh.HasTextureCoords(0)) {

		num_texcoords = num_vertices;
		float* temp_texcoords = new float[num_texcoords * 3];

		memcpy(temp_texcoords, imp_mesh.mTextureCoords[0], sizeof(float3) * num_texcoords);

		texcoords = new float2[num_texcoords];

		for (int i = 0; i < num_texcoords; i++)    // remove the 0 value from assimp
		{
			texcoords[i].x = temp_texcoords[i * 3];
			texcoords[i].y = temp_texcoords[i * 3 + 1];
		}

		delete[] temp_texcoords;
	}

	return true;
}


void ComponentMesh::LoadDataToVRAM() {

	if (num_tris > 0)
	{
		glGenBuffers(1, (GLuint*) &(id_tris));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_tris);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Tri) * num_tris, tris, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	if (num_vertices > 0) {

		glGenBuffers(1, (GLuint*) &(id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * num_vertices, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	if (num_normals > 0) {

		glGenBuffers(1, (GLuint*) &(id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * num_normals, normals, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	if (num_texcoords > 0) {
		glGenBuffers(1, (GLuint*) &(id_texcoords));
		glBindBuffer(GL_ARRAY_BUFFER, id_texcoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * num_texcoords, texcoords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}


void ComponentMesh::Draw() {

	if(mat)
		glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_tris);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, id_texcoords);
	glBindBuffer(GL_NORMAL_ARRAY, id_normals);

	glColor3f(1.0f, 1.0f, 1.0f);

	if (mat)
		glBindTexture(GL_TEXTURE_2D, mat->textures.back()->gl_binding);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	glNormalPointer(GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, num_tris * 3, GL_UNSIGNED_INT, NULL);

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


void ComponentMesh::Save(JSON_file& save_file, const char* component_code)
{
	std::string attribute_code(component_code);
	transform->Save(save_file, attribute_code.append(".transform").c_str());
}

void ComponentMesh::Load(JSON_file& save_file, const char* component_code)
{
	std::string attribute_code(component_code);
	transform->Load(save_file, attribute_code.append(".transform").c_str());
}