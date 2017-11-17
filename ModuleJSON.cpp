#include "ModuleJSON.h"
#include "Application.h"

#include <fstream>
#include <iostream>

ModuleJSON::ModuleJSON(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleJSON::~ModuleJSON() {}

JSON_file* ModuleJSON::LoadFile(const char* path)
{
	JSON_Value *user_data = json_parse_file(path);
	JSON_Object *root_object = json_value_get_object(user_data);
	JSON_file* file = nullptr;

	if (user_data && root_object)
	{
		App->gui->app_log.AddLog("Succesfully loaded %s file\n", path);
		file = new JSON_file(user_data, root_object, path);
	}
	else
		App->gui->app_log.AddLog("Error loading %s file!\n", path);

	return file;
}

JSON_file* ModuleJSON::OpenFile(const char* filename, const char* write_dir)
{
	JSON_file* file = nullptr;

	std::string full_path(write_dir);
	full_path.append("\\");
	full_path.append(filename);

	if (!App->fs->Exists(full_path.c_str()))
	{
		if (App->fs->Save(filename, "{\n}", write_dir, 4) == 0)
			App->gui->app_log.AddLog("Error creating %s file in %s directory !\n", filename, write_dir);
	}

	if(!(file = LoadFile(full_path.c_str())));
		App->gui->app_log.AddLog("Error loading %s file in %s directory !\n", filename, write_dir);

	return file;
}

JSON_file::JSON_file(JSON_Value * value, JSON_Object * object, const char * path) : value(value), object(object), path(path)
{}

JSON_file::~JSON_file()
{}

void JSON_file::WriteString(const char* name, const char* value)
{
	json_object_dotset_string(object, name, value);
}

void JSON_file::WriteBool(const char* name, bool value)
{
	json_object_dotset_boolean(object, name, value);
}

void JSON_file::WriteNumber(const char* name, float value)
{
	json_object_dotset_number(object, name, value);
}

void JSON_file::WriteColor(const char* name, Color value)
{
	std::string s; 

	s.append(name); s.append(".r");
	json_object_dotset_number(object, s.c_str(), value.r);

	s.clear(); s.append(name); s.append(".g");
	json_object_dotset_number(object, s.c_str(), value.g);

	s.clear(); s.append(name); s.append(".b");
	json_object_dotset_number(object, s.c_str(), value.b);

	s.clear(); s.append(name); s.append(".a");
	json_object_dotset_number(object, s.c_str(), value.a);
}

void JSON_file::WriteFloat3(const char* name, float3 value)
{
	std::string s;

	s.append(name); s.append(".x");
	json_object_dotset_number(object, s.c_str(), value.x);

	s.clear(); s.append(name); s.append(".y");
	json_object_dotset_number(object, s.c_str(), value.y);

	s.clear(); s.append(name); s.append(".z");
	json_object_dotset_number(object, s.c_str(), value.z);
}

const char * JSON_file::ReadString(const char* name)
{
	JSON_Value* value = json_object_dotget_value(object, name);

	if (value != nullptr && json_value_get_type(value) == json_value_type::JSONString)
		return json_object_dotget_string(object, name);
	else
		return ("");
}

bool JSON_file::ReadBool(const char * name)
{
	JSON_Value* value = json_object_dotget_value(object, name);

	if (value != nullptr && json_value_get_type(value) == json_value_type::JSONBoolean)
		return json_object_dotget_boolean(object, name);
	else
		return -1;
}

float JSON_file::ReadNumber(const char * name)
{
	JSON_Value* value = json_object_dotget_value(object, name);

	if (value != nullptr && json_value_get_type(value) == json_value_type::JSONNumber)
		return json_object_dotget_number(object, name);
	else
		return -1;
}

Color JSON_file::ReadColor(const char* name)
{
	std::string s; Color ret = {0.0f, 0.0f, 0.0f, 0.0f};

	s.append(name); s.append(".r");
	ret.r = json_object_dotget_number(object, s.c_str());

	s.clear(); s.append(name); s.append(".g");
	ret.g = json_object_dotget_number(object, s.c_str());

	s.clear(); s.append(name); s.append(".b");
	ret.b = json_object_dotget_number(object, s.c_str());

	s.clear(); s.append(name); s.append(".a");
	ret.a = json_object_dotget_number(object, s.c_str());

	return ret;
}

float3 JSON_file::ReadFloat3(const char* name)
{
	std::string s; float3 ret = { -1.0f, -1.0f, -1.0f};

	s.append(name); s.append(".x");
	ret.x = json_object_dotget_number(object, s.c_str());

	s.clear(); s.append(name); s.append(".y");
	ret.y = json_object_dotget_number(object, s.c_str());

	s.clear(); s.append(name); s.append(".z");
	ret.z = json_object_dotget_number(object, s.c_str());

	return ret;
}


void JSON_file::Save()
{
	json_serialize_to_file_pretty(value, path.c_str());
}

void JSON_file::CleanUp()
{
	json_value_free(value);
}
