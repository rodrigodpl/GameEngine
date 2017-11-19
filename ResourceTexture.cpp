#include "ResourceTexture.h"

#include "glew-2.1.0\include\GL\glew.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ResourceTexture::ResourceTexture(std::string uid) : Resource(uid, Res_Type::texture)
{}


ResourceTexture::~ResourceTexture() 
{
	UnLoadToMemory();
}

void ResourceTexture::UnLoadToMemory()
{
	if (gpu_id)
		glDeleteTextures(1, &gpu_id);
}

void ResourceTexture::LoadToMemory() 
{
	std::string full_path(uid);

	full_path.insert(0, "/");
	full_path.insert(0, LIBRARY_TEXTURES_PATH);

	if (gpu_id == 0)
		gpu_id = ilutGLLoadImage((char*)full_path.c_str());

}


//void Save(Config& config) const override;
//void Load(const Config& config) override;