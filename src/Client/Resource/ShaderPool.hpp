#ifndef SHADERPOOL_HPP
#define SHADERPOOL_HPP

#include "Common/Resource/ResourcePool.hpp"
#include "Client/Rendering/ShaderProgram.hpp"

//Loads and stores all textures using in game.
class ShaderPool : public ResourcePool<ShaderProgram*>
{
public:
	static ShaderPool*  getInstance();

protected:
	//Instance for the Singleton design pattern;
	static ShaderPool* instance;
	ShaderPool();

	virtual ShaderProgram* loadResource(string name);
	virtual void unloadResource(ShaderProgram* resource);
};
#endif //SHADERPOOL_HPP