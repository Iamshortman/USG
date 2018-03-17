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

void MeshPool::loadMesh(string filename)
{
	if (this->hasMesh(filename))
	{
		//Already loaded
		return;
	}

	StaticMeshResource resource;
	resource.usingCount = 0;
	resource.mesh = nullptr;

	this->staticMeshes[filename] = resource;
}

void MeshPool::unloadMesh(string filename)
{
	if (this->hasMesh(filename))
	{
		delete this->staticMeshes[filename].mesh;
		this->staticMeshes[filename].mesh = nullptr;
		//this->staticMeshes.erase(filename);
	}
}

void MeshPool::setUsing(string filename)
{
	if (this->hasMesh(filename))
	{
		if (this->staticMeshes[filename].mesh == nullptr)
		{
			this->staticMeshes[filename].mesh = TexturedMesh::loadObj(filename);
		}
		this->staticMeshes[filename].usingCount++;
	}
}

int MeshPool::getUsing(string filename)
{
	//Return -1 if it is not loaded
	if (!this->hasMesh(filename))
	{
		return -1;
	}

	return this->staticMeshes[filename].usingCount;
}

void MeshPool::releaseUsing(string filename)
{
	//Do nothing if it's not loaded
	if (!this->hasMesh(filename))
	{
		return;
	}

	this->staticMeshes[filename].usingCount--;

	//If no one is using it, unload it
	if (this->staticMeshes[filename].usingCount <= 0)
	{
		this->unloadMesh(filename);
	}
}

Mesh* MeshPool::getMesh(string filename)
{
	//Does not contain
	if (!this->hasMesh(filename))
	{
		return nullptr;
	}

	return this->staticMeshes[filename].mesh;
}

bool MeshPool::hasMesh(string filename)
{
	return this->staticMeshes.find(filename) != this->staticMeshes.end();
}
