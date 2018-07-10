#include "Client/Resource/TexturePool.hpp"

#include "Common/Logger/Logger.hpp"

TexturePool* TexturePool::instance = nullptr;
TexturePool* TexturePool::getInstance()
{
	if (TexturePool::instance == nullptr)
	{
		TexturePool::instance = new TexturePool();
	}

	return TexturePool::instance;
}

TexturePool::TexturePool()
{

}

GLuint TexturePool::loadResource(string name)
{
	int width, height, numComponents;
	unsigned char* data = load_png((name).c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
	{
		Logger::getInstance()->logError("Can't Load the given texture: %s\n", name.c_str());
		return 0;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	unload_png(data);

	return texture;
}

void TexturePool::unloadResource(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

#define STB_IMAGE_IMPLEMENTATION
#include "Client/Resource/stb_image.h"

unsigned char* load_png(char const *filename, int *x, int *y, int *comp, int req_comp)
{
	return stbi_load(filename, x, y, comp, req_comp);
}

void unload_png(void *retval_from_stbi_load)
{
	stbi_image_free(retval_from_stbi_load);
}