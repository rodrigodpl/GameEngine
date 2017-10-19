#include "ComponentMaterial.h"

#include "glew-2.1.0\include\GL\glew.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

ComponentMaterial::ComponentMaterial() 
{
	type = Component_type::COMPONENT_MATERIAL;
	CheckeredTexture();
}

ComponentMaterial::ComponentMaterial(const char* tex_full_path) {

	Texture* tex = new Texture();
	tex->gl_binding = ilutGLLoadImage((char*)tex_full_path);

	tex->width = ilGetInteger(IL_IMAGE_WIDTH);
	tex->height = ilGetInteger(IL_IMAGE_HEIGHT);

	tex->path = tex_full_path;

	textures.push_back(tex);

}

ComponentMaterial::ComponentMaterial(aiMaterial& mat) {

	type = Component_type::COMPONENT_MATERIAL;

	uint numTextures = mat.GetTextureCount(aiTextureType_DIFFUSE);

	for (int i = 0; i < numTextures; i++) {

		Texture* tex = new Texture();
		mat.GetTexture(aiTextureType_DIFFUSE, i, &tex->path);
	
		tex->gl_binding = ilutGLLoadImage((char*)tex->path.C_Str());

		if (tex->gl_binding != 0) {
			tex->width = ilGetInteger(IL_IMAGE_WIDTH);
			tex->height = ilGetInteger(IL_IMAGE_HEIGHT);

			textures.push_back(tex);
		}
		else
			CheckeredTexture();
	}

	if (textures.empty())
		CheckeredTexture();

}

ComponentMaterial::ComponentMaterial(ComponentMaterial& component) {

	type = Component_type::COMPONENT_MATERIAL;

	for (std::vector<Texture*>::iterator it = component.textures.begin(); it != component.textures.end(); it++) {

		Texture* tex = new Texture();
		tex->gl_binding = ilutGLLoadImage((char*) (*it)->path.C_Str());

		if (tex->gl_binding != 0) {
			tex->width = ilGetInteger(IL_IMAGE_WIDTH);
			tex->height = ilGetInteger(IL_IMAGE_HEIGHT);

			textures.push_back(tex);
		}
		else
			CheckeredTexture();
	}
}


ComponentMaterial::~ComponentMaterial() {

	for (std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++) 
		glDeleteTextures(1, &(*it)->gl_binding);

	textures.clear();

}

Component* ComponentMaterial::Duplicate()
{
	return (Component*) new ComponentMaterial(*this);
}


void ComponentMaterial::CheckeredTexture() {

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {

			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	Texture* checkered_tex = new Texture();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &checkered_tex->gl_binding);
	glBindTexture(GL_TEXTURE_2D, checkered_tex->gl_binding);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	checkered_tex->width = CHECKERS_WIDTH;
	checkered_tex->height = CHECKERS_HEIGHT;

	glBindTexture(GL_TEXTURE_2D, 0);

	textures.push_back(checkered_tex);
}


