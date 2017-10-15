#include "ModuleJSON.h"
#include "Application.h"

ModuleJSON::ModuleJSON(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleJSON::~ModuleJSON() {}


bool ModuleJSON::Start() {

	JSON_file* config = LoadFile("config.json");

	config->WriteString("test", "retest");
	config->Save();

	return true;
}

bool ModuleJSON::CleanUp() {

	return true;
}

JSON_file* ModuleJSON::LoadFile(const char * path)
{

	for (std::list<JSON_file*>::iterator it = files.begin(); it != files.end(); it++)
	{
		if ((*it)->path == path)
			return (*it);        // break loop if file is already loaded
	}

	JSON_Value *user_data = json_parse_file(path);
	JSON_Object *root_object = json_value_get_object(user_data);

	if (user_data && root_object)
	{
		JSON_file* new_file = new JSON_file(user_data, root_object, path);
		files.push_back(new_file);
		return new_file;
	}
	else
		App->gui->app_log.AddLog("Error loading %s file!", path);

	return nullptr;
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
