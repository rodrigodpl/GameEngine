#pragma once

#include "Module.h"

struct SDL_RWops;
class Color;

int close_sdl_rwops(SDL_RWops *rw);


#include <vector>
#include <string>

class ModuleFS : public Module
{
public:
	char* base_path;

public:

	ModuleFS(Application* app, bool start_enabled = true);
	~ModuleFS();

	bool AddPath(const char* path_or_zip, const char* mount_point = NULL);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;

	char* Load(const char* file, uint& size) const;
	SDL_RWops* Load(const char* file) const;

	std::vector<std::string> GetFilesInDir(const char* dir, bool append_root_folder = false);
	unsigned int Save(const char* file_name, const void* data, const char* write_dir, unsigned int size) const;
	void CreateDir(const char* path, bool hidden = false);

};
