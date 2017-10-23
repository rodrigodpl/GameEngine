#include "Importer.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"


bool MaterialImporter::Import(const char* file, const char* path, std::string& output_file) {

	bool ret = false;

	//ILuint size;
	//ILubyte *data;
	//ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	//size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	//if (size > 0) {
	//	data = new ILubyte[size]; // allocate data buffer
	//	if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
	//		ret = App->fs->SaveUnique(output_file, data, size, LIBRARY_TEXTURES_FOLDER, "texture", "dds");
	//	RELEASE_ARRAY(data);
	//}

	return ret;

}