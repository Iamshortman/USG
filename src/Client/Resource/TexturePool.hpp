#ifndef TEXTUREPOOL_HPP
#define TEXTUREPOOL_HPP

#include <unordered_map>
#include <string>
#include <iostream>

#include "Client/Rendering/OpenGL_Include.hpp"

using std::string;
/*
//Loads and stores all textures using in game.
class TexturePool
{

public:
	static TexturePool* instance;

	TexturePool();
	~TexturePool();
	void loadTexture(string fileLoc);
	void unloadTexture(string fileLoc);
	bool bindTexture(int activeTexture, string fileLoc);

private:
	std::unordered_map<string, GLuint> textures;
};*/

struct TextureResource
{
	int usingCount = 0;
	GLuint texture;
};

//Loads and stores all textures using in game.
class TexturePool
{

public:
	static TexturePool* instance;

	TexturePool();
	~TexturePool();
	void loadTexture(string filename);
	void unloadTexture(string name);

	void setUsing(string name);
	int getUsing(string name);
	void releaseUsing(string name);
	GLuint getTexture(string name);

private:
	std::unordered_map<string, TextureResource> textures;
};

#endif //TEXTUREPOOL_HPP