#pragma once
#include "Module.h"
#include "parson-master\parson.h"
#include "Color.h"

#include<string>
#include<list>


class JSON_file
{
public:
	JSON_file(JSON_Value* value, JSON_Object* object, const char* path);
	~JSON_file();

	void		WriteString	(const char* name, const char* value);
	void		WriteBool	(const char* name, bool value);
	void		WriteNumber	(const char* name, float value);
	void		WriteColor	(const char* name, Color value);
										 
	const char* ReadString	(const char* name);
	bool		ReadBool	(const char* name);
	float		ReadNumber	(const char* name);
	Color		ReadColor   (const char* name);

	void Save();
	void CleanUp();

public:
	JSON_Value* value = nullptr;
	JSON_Object* object = nullptr;
	std::string path;
};


class ModuleJSON : public Module
{
public:
	ModuleJSON(Application* app, bool start_enabled = true);
	~ModuleJSON();

	JSON_file* LoadFile(const char* path);
	JSON_file* OpenFile(const char* filename, const char* write_dir);    // if file does not exist, it will create it
	
};