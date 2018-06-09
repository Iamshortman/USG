#ifndef TEXTUREPOOL_HPP
#define TEXTUREPOOL_HPP

#include "Common/Resource/ResourcePool.hpp"
#include "Client/Rendering/OpenGL_Include.hpp"

//Loads and stores all textures using in game.
class TexturePool : public ResourcePool<GLuint>
{
public:
	static TexturePool*  getInstance();

protected:
	//Instance for the Singleton design pattern;
	static TexturePool* instance;
	TexturePool();

	virtual GLuint loadResource(string name);
	virtual void unloadResource(GLuint texture);
};


unsigned char* load_png(char const *filename, int *x, int *y, int *comp, int req_comp);
void unload_png(void *retval_from_stbi_load);

#endif //TEXTUREPOOL_HPP