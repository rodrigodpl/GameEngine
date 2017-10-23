#pragma once
#include "Globals.h"
#include "ComponentMaterial.h"

#include <string>

class Importer {



};


class MaterialImporter : public Importer
{
public:
	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file, Texture* resource);
	bool LoadCheckers(Texture* resource);
};