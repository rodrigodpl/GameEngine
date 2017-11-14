#pragma once

#include "Module.h"

struct SDL_RWops;

int close_sdl_rwops(SDL_RWops *rw);

#define LIBRARY_BASE_PATH "Library"
#define LIBRARY_MESHES_PATH "Library/Meshes"
#define LIBRARY_TEXTURES_PATH "Library/Textures"
#define ASSETS_BASE_PATH "Assets"

#include <vector>
#include <string>

class ModuleFS : public Module
{
public:
	char* base_path;
	char* write_path;

public:

	ModuleFS(Application* app, bool start_enabled = true);
	~ModuleFS();

	bool Start();
	bool CleanUp();

	bool AddPath(const char* path_or_zip, const char* mount_point = NULL);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;

	unsigned int Load(const char* file, char* buffer) const;
	SDL_RWops* Load(const char* file) const;

	std::vector<std::string> GetFilesInDir(const char* dir, bool append_root_folder = false);
	unsigned int Save(const char* file_name, const char* data, const char* write_dir, unsigned int size) const;
	void CreateDir(const char* path, bool hidden);
};
