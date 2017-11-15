#include "Application.h"
#include "ModuleFS.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

#include "PhysFS\include\physfs.h"

#include <string>

ModuleFS::ModuleFS(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);

	AddPath(".");

	CreateDir(LIBRARY_BASE_PATH, true);
	CreateDir(LIBRARY_MESHES_PATH);
	CreateDir(LIBRARY_TEXTURES_PATH);
	CreateDir(SETTINGS_BASE_PATH);

}

ModuleFS::~ModuleFS()
{
	PHYSFS_deinit();
}

bool ModuleFS::Start()
{
	App->gui->app_log.AddLog("Loading File System\n");
	return true;
}

bool ModuleFS::CleanUp()
{
	App->gui->app_log.AddLog("Freeing File System subsystem\n");
	return true;
}

// Add a new zip file or folder
bool ModuleFS::AddPath(const char* path_or_zip, const char* mount_point)
{
	bool ret = false;

	if(PHYSFS_mount(path_or_zip, mount_point, 1) == 0)
		App->gui->app_log.AddLog("File System error while adding a path or zip(%s): %s\n", path_or_zip, PHYSFS_getLastError());
	else
		ret = true;

	return ret;
}

bool ModuleFS::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

bool ModuleFS::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

// Read a whole file and put it in a new buffer
unsigned int ModuleFS::Load(const char* file, char* buffer) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if(fs_file != NULL)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);

		if(size > 0)
		{
			buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fs_file, buffer, 1, (PHYSFS_sint32)size);
			if(readed != size)
			{
				App->gui->app_log.AddLog("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				if (buffer)
					delete[] buffer;
			}
			else
				ret = (uint)readed;
		}

		if(PHYSFS_close(fs_file) == 0)
			App->gui->app_log.AddLog("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		App->gui->app_log.AddLog("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

// Read a whole file and put it in a new buffer
SDL_RWops* ModuleFS::Load(const char* file) const
{
	char* buffer;
	int size = Load(file, buffer);

	if(size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if(r != NULL)
			r->close = close_sdl_rwops;

		return r;
	}
	else
		return NULL;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	if (rw->hidden.mem.base)
		delete rw->hidden.mem.base;

	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
unsigned int ModuleFS::Save(const char* file_name, const char* data, const char* write_dir, unsigned int size)const
{
	unsigned int ret = 0;

	if (PHYSFS_setWriteDir(write_dir) != 0) {

		PHYSFS_file* fs_file = PHYSFS_openWrite(file_name);

		if (fs_file != NULL)
		{
			PHYSFS_sint64 written = PHYSFS_write(fs_file, (const void*)data, 1, size);
			if (written != size)
				App->gui->app_log.AddLog("File System error while writing to file %s: %s\n", file_name, PHYSFS_getLastError());
			else
				ret = (uint)written;

			if (PHYSFS_close(fs_file) == 0)
				App->gui->app_log.AddLog("File System error while closing file %s: %s\n", file_name, PHYSFS_getLastError());
		}
		else
			App->gui->app_log.AddLog("File System error while opening file %s: %s\n", file_name, PHYSFS_getLastError());
	}
	else
		App->gui->app_log.AddLog("File System error while setting write dir  %s: %s\n", write_dir, PHYSFS_getLastError());

	return ret;
}


void ModuleFS::CreateDir(const char* path, bool hidden)
{
	DWORD ftyp = GetFileAttributesA(path);
	LPCSTR file = path;
	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(file, NULL);
		if (hidden) SetFileAttributes(file, FILE_ATTRIBUTE_HIDDEN);
	}

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY);
	//It exists
}

std::vector<std::string> ModuleFS::GetFilesInDir(const char* dir, bool append_root_folder)
{
	std::vector<std::string> files;
	char** assets = PHYSFS_enumerateFiles(dir);

	if (assets != nullptr) {

		for (int i = 0; assets[i] != nullptr; i++)
		{
			std::string path(assets[i]);

			if (append_root_folder) {
				path.insert(0, "/");
				path.insert(0, dir);
			}

			files.push_back(path);
		}
	}
	PHYSFS_freeList(assets);
	return files;
}
