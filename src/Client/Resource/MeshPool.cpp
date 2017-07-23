#include "Client/Resource/MeshPool.hpp"

MeshPool* MeshPool::instance;

MeshPool::MeshPool()
{
	instance = this;
}

MeshPool::~MeshPool()
{
	for (auto it = this->staticMeshes.begin(); it != this->staticMeshes.end(); ++it)
	{
		delete it->second.mesh;
	}
}

void MeshPool::loadModel(string name, string filename, bool isStatic)
{
	if (this->staticMeshes.count(name))
	{
		//Already loaded
		return;
	}

	if (isStatic)
	{
		StaticMeshResource resource;
		resource.usingCount = 0;
		resource.mesh = TexturedMesh::loadObj(filename);

		if (resource.mesh != nullptr)
		{
			this->staticMeshes[name] = resource;
		}
	}
	else
	{
		StaticMeshResource resource;
		resource.usingCount = 0;
		resource.mesh = AnimatedMesh::loadMesh(filename);

		this->staticMeshes[name] = resource;
	}
}

void MeshPool::unloadModel(string name)
{
	if (this->staticMeshes.count(name))
	{
		delete this->staticMeshes[name].mesh;
		this->staticMeshes.erase(name);
	}
}

void MeshPool::setUsing(string name)
{
	//TODO, load the shader if it doesn't exsist
	if (!this->staticMeshes.count(name))
	{
		//TODO load here
		return;
	}

	this->staticMeshes[name].usingCount++;
}

int MeshPool::getUsing(string name)
{
	//Return -1 if it is not loaded
	if (!this->staticMeshes.count(name))
	{
		return -1;
	}

	return this->staticMeshes[name].usingCount;
}

void MeshPool::releaseUsing(string name)
{
	//Do nothing if it's not loaded
	if (!this->staticMeshes.count(name))
	{
		return;
	}

	this->staticMeshes[name].usingCount--;

	//If no one is using it, unload it
	if (this->staticMeshes[name].usingCount <= 0)
	{
		this->unloadModel(name);
	}
}

Mesh* MeshPool::getModel(string name)
{
	//Does not contain
	if (!this->staticMeshes.count(name))
	{
		return nullptr;
	}

	return staticMeshes[name].mesh;
}