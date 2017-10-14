
#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleSceneIntro.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")


ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleImporter::~ModuleImporter() {}

bool ModuleImporter::Start() {

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	LoadTex("test");

	return true;
}

bool ModuleImporter::CleanUp() {

	aiDetachAllLogStreams();

	return true;  
}

void ModuleImporter::LoadTex(const char* full_path) {

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &image_id);
	glBindTexture(GL_TEXTURE_2D, image_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);


}

void ModuleImporter::LoadFBX(const char* full_path) {

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++) {

			Mesh mesh;
			aiMesh* imp_mesh = scene->mMeshes[i];

			if (imp_mesh->HasPositions()) {
				mesh.num_vertices = imp_mesh->mNumVertices;
				mesh.vertices = new float[mesh.num_vertices * 3];
				memcpy(mesh.vertices, imp_mesh->mVertices, sizeof(float) * mesh.num_vertices * 3);

				glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
				glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, &mesh.vertices[0], GL_STATIC_DRAW);

				App->gui->app_log.AddLog("New mesh with %d vertices", mesh.num_vertices);
			}

			if (imp_mesh->HasNormals()) {

				mesh.num_normals = mesh.num_vertices;
				mesh.normals = new float[mesh.num_normals * 3];
				memcpy(mesh.normals, imp_mesh->mNormals, sizeof(float) * mesh.num_normals * 3);

				glGenBuffers(1, (GLuint*) &(mesh.id_normals));
				glBindBuffer(GL_NORMAL_ARRAY, mesh.id_normals);
				glBufferData(GL_NORMAL_ARRAY, sizeof(float) * mesh.num_normals * 3, &mesh.normals[0], GL_STATIC_DRAW);

				App->gui->app_log.AddLog("New mesh with %d normals", mesh.num_normals);
			}

			int a = imp_mesh->GetNumUVChannels();

			if (imp_mesh->HasTextureCoords(0)) {

				mesh.num_texcoords = mesh.num_vertices;
				mesh.texcoords = new float[mesh.num_texcoords * 2];
				memcpy(mesh.texcoords, imp_mesh->mTextureCoords[0], sizeof(float) * mesh.num_texcoords * 2);

				glGenBuffers(1, (GLuint*) &(mesh.id_texcoords));
				glBindBuffer(GL_TEXTURE_COORD_ARRAY, mesh.id_texcoords);
				glBufferData(GL_TEXTURE_COORD_ARRAY, sizeof(float) * mesh.num_texcoords * 2, &mesh.texcoords[0], GL_STATIC_DRAW);

				App->gui->app_log.AddLog("New mesh with %d texcoords", mesh.num_texcoords);

			}

			if (imp_mesh->HasFaces())
			{
				mesh.num_indices = imp_mesh->mNumFaces * 3;
				mesh.indices = new uint[mesh.num_indices]; // assume each face is a triangle
				for (uint i = 0; i < imp_mesh->mNumFaces; ++i)
				{
					if (imp_mesh->mFaces[i].mNumIndices != 3)
						App->gui->app_log.AddLog("WARNING, geometry face with != 3 indices!");
					else
						memcpy(&mesh.indices[i * 3], imp_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}

				glGenBuffers(1, (GLuint*) &(mesh.id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, &mesh.indices[0], GL_STATIC_DRAW);

				App->gui->app_log.AddLog("New mesh with %d indices", mesh.num_indices);
			}

			// load colors
			
			App->scene_intro->meshes.push_back(mesh);

		}

		aiReleaseImport(scene);
	}
	else
		App->gui->app_log.AddLog("Error loading scene %s", full_path);

}
