#pragma once
#include "Globals.h"

#include <string>

enum Res_Type {
	texture,
	mesh,
	audio,
	scene,
	bone,
	animation,
	unknown
};

class Resource {

public:
	Resource(std::string uid, Res_Type type);
	virtual ~Resource();

	Res_Type GetType() const;
	std::string GetUID() const;
	const char* GetImportedFile() const;

	virtual void LoadToMemory()  {}
	virtual void UnLoadToMemory() {}

	//virtual void Save(Config& config) const;
	//virtual void Load(const Config& config);

	uint AddInstance()		{ return ++instances; };
	uint RemoveInstance()	{ return --instances; };
	uint Instances()		{ return instances; };

protected:

	std::string uid;
	std::string imported_file;
	Res_Type type = Res_Type::unknown;
	uint instances = 0;
};
