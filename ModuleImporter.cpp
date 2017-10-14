
#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleSceneIntro.h"

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleImporter::~ModuleImporter() {}

bool ModuleImporter::Start() {

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleImporter::CleanUp() {

	aiDetachAllLogStreams();

	return true;  
}

void ModuleImporter::LoadFBX(const char* full_path) {

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++) {

			aiMesh* imp_mesh = scene->mMeshes[i];
			Mesh mesh;

			mesh.num_vertices = imp_mesh->mNumVertices;
			mesh.vertices = new float[mesh.num_vertices * 3];
			memcpy(mesh.vertices, imp_mesh->mVertices, sizeof(float) * mesh.num_vertices * 3);
			App->gui->app_log.AddLog("New mesh with %d vertices", mesh.num_vertices);


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
			}

			glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, &mesh.vertices[0], GL_STATIC_DRAW);

			glGenBuffers(1, (GLuint*) &(mesh.id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, &mesh.indices[0], GL_STATIC_DRAW);

			App->scene_intro->meshes.push_back(mesh);

		}

		aiReleaseImport(scene);
	}
	else
		App->gui->app_log.AddLog("Error loading scene %s", full_path);

}
