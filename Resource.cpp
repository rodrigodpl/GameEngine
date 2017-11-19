#include "Resource.h"


Resource::Resource(std::string uid, Res_Type type) : uid(uid), type(type) {}

Resource::~Resource() {}

std::string Resource::GetUID() const
{
	return uid;
}

const char* Resource::GetImportedFile() const
{
	return imported_file.c_str();
}

Res_Type Resource::GetType() const 
{
	return type;
}