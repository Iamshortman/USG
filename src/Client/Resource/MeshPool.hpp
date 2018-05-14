#ifndef MESHPOOL_HPP
#define MESHPOOL_HPP

#include "Common/Resource/ResourcePool.hpp"
#include "Client/Resource/TexturedMesh.hpp"

//Loads and stores all textures using in game.
class MeshPool : public ResourcePool<TexturedMesh*>
{
public:
	static MeshPool*  getInstance();

protected:
	//Instance for the Singleton design pattern;
	static MeshPool* instance;
	MeshPool();

	virtual TexturedMesh* loadResource(string name);
	virtual void unloadResource(TexturedMesh* resource);
};

#endif //MESHPOOL_HPP