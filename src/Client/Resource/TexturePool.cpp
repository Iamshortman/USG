#include "Client/Resource/TexturePool.hpp"

TexturePool* TexturePool::instance;

//Prototype Functions
GLuint ReadTexture(string filename);
void DeleteTexture(GLuint texture);

TexturePool::TexturePool()
{
	instance = this;
}

TexturePool::~TexturePool()
{
	for (auto it = this->textures.begin(); it != this->textures.end(); ++it)
	{
		this->unloadTexture(it->first);
	}
}

void TexturePool::loadTexture(string filename)
{
	//Does contain
	if (this->textures.count(filename))
	{
		//Already loaded
		return;
	}

	//TODO load PNG
	TextureResource resource;
	resource.texture = ReadTexture(filename);

	this->textures[filename] = resource;
}

void TexturePool::setUsing(string name)
{
	//TODO, load the texture if it doesn't exsist
	//Does not contain
	if (!this->textures.count(name))
	{
		//TODO load here
		return;
	}

	this->textures[name].usingCount++;
}

int TexturePool::getUsing(string name)
{
	//Return -1 if it is not loaded
	//Does not contain
	if (!this->textures.count(name))
	{
		return -1;
	}

	return this->textures[name].usingCount;
}

void TexturePool::releaseUsing(string name)
{
	//Does not contain
	if (!this->textures.count(name))
	{
		return;
	}

	this->textures[name].usingCount--;

	//If no one is using it, unload it
	if (this->textures[name].usingCount <= 0)
	{
		this->unloadTexture(name);
	}
}

void TexturePool::unloadTexture(string name)
{
	//Does contain
	if (this->textures.count(name))
	{
		//TODO unload Texture
		DeleteTexture(this->textures[name].texture);
		this->textures.erase(name);
	}
}

GLuint TexturePool::getTexture(string name)
{
	//Does not contain
	if (!this->textures.count(name))
	{
		return 0;
	}

	return textures[name].texture;
}

//TODO Move to LIB location
#define STB_IMAGE_IMPLEMENTATION
#include "Client/Resource/stb_image.h"

GLuint ReadTexture(string filename)
{
	int width, height, numComponents;
	unsigned char* data = stbi_load((filename).c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
	{
		std::cerr << "Can't Load the given texture: " << filename << std::endl;
		return 0;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f);

	stbi_image_free(data);

	return textureId;
}

void DeleteTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}