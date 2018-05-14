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

#define STB_IMAGE_IMPLEMENTATION
#include "Client/Resource/stb_image.h"

GLuint TexturePool::loadResource(string name)
{
	int width, height, numComponents;
	unsigned char* data = stbi_load((name).c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
	{
		Logger::getInstance()->logError("Can't Load the given texture: %s\n", name);
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
	stbi_image_free(data);

	return texture;
}

void TexturePool::unloadResource(GLuint texture)
{
	glDeleteTextures(1, &texture);
}
