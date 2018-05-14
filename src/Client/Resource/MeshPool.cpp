#include "Client/Resource/MeshPool.hpp"

#include "Common/Logger/Logger.hpp"

MeshPool* MeshPool::instance = nullptr;
MeshPool* MeshPool::getInstance()
{
	if (MeshPool::instance == nullptr)
	{
		MeshPool::instance = new MeshPool();
	}

	return MeshPool::instance;
}

MeshPool::MeshPool()
{

}

TexturedMesh* MeshPool::loadResource(string name)
{
	return TexturedMesh::loadObj(name);
}

void MeshPool::unloadResource(TexturedMesh* resource)
{
	delete resource;
}