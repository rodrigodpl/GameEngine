#include "ModuleJSON.h"
#include "Application.h"

#include <fstream>
#include <iostream>

ModuleJSON::ModuleJSON(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleJSON::~ModuleJSON() {}


bool ModuleJSON::Start() {
	std::fstream appendFileToWorkWith;

	appendFileToWorkWith.open("config.json", std::fstream::in | std::fstream::out | std::fstream::app);

	if (!appendFileToWorkWith)
	{
		App->gui->app_log.AddLog("Config file not found\n Creating config file\n");

		appendFileToWorkWith.open("config.json", std::fstream::in | std::fstream::out | std::fstream::trunc);
		appendFileToWorkWith << "{\n";
		appendFileToWorkWith << "}\n";
		appendFileToWorkWith.close();

	}
	else
	{
		App->gui->app_log.AddLog("Config file found\n");

		appendFileToWorkWith.close();
	}

	App->gui->app_log.AddLog("Loading config file\n");

	config = LoadFile("config.json");

	return true;
}

bool ModuleJSON::CleanUp() {

	config->Save();

	return true;
}

JSON_file* ModuleJSON::LoadFile(const char* path)
{

	JSON_Value *user_data = json_parse_file(path);
	JSON_Object *root_object = json_value_get_object(user_data);

	if (user_data && root_object)
	{
		return config = new JSON_file(user_data, root_object, path);
		App->gui->app_log.AddLog("Succesfully loaded %s file\n", path);
	}
	else
		App->gui->app_log.AddLog("Error loading %s file!\n", path);

	return nullptr;
}

JSON_file* ModuleJSON::OpenFile(const char* filename, const char* write_dir)
{
	JSON_file* file = nullptr;
	if (!App->fs->Exists(filename))
	{
		if (App->fs->Save(filename, "{\n}", write_dir, 4))
		{
			std::string full_path(write_dir);
			full_path.append("\\");
			full_path.append(filename);

			file = LoadFile(full_path.c_str());
		}
		else
			App->gui->app_log.AddLog("Error creating %s file in %s directory !\n", filename, write_dir);
	}

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

const char * JSON_file::ReadString(const char* name)
{
	JSON_Value* value = json_object_dotget_value(object, name);

	if (value != nullptr && json_value_get_type(value) == json_value_type::JSONString)
		return json_object_dotget_string(object, name);
	else
		return ("unable to find string");
}

bool JSON_file::ReadBool(const char * name)
{
	JSON_Value* value = json_object_dotget_value(object, name);

	if (value != nullptr && json_value_get_type(value) == json_value_type::JSONBoolean)
		return json_object_dotget_boolean(object, name);
	else
		return false;
}

float JSON_file::ReadNumber(const char * name)
{
	JSON_Value* value = json_object_dotget_value(object, name);

	if (value != nullptr && json_value_get_type(value) == json_value_type::JSONNumber)
		return json_object_dotget_number(object, name);
	else
		return -1;
}

void JSON_file::Save()
{
	json_serialize_to_file_pretty(value, path.c_str());
}

void JSON_file::CleanUp()
{
	json_value_free(value);
}
