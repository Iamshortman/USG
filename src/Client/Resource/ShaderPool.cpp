#include "Client/Resource/ShaderPool.hpp"

#include "Common/Logger/Logger.hpp"

ShaderPool* ShaderPool::instance = nullptr;
ShaderPool* ShaderPool::getInstance()
{
	if (ShaderPool::instance == nullptr)
	{
		ShaderPool::instance = new ShaderPool();
	}

	return ShaderPool::instance;
}

ShaderPool::ShaderPool()
{

}

ShaderProgram* ShaderPool::loadResource(string name)
{
	return new ShaderProgram(name + ".vs", name + ".fs");
}

void ShaderPool::unloadResource(ShaderProgram* resource)
{
	delete resource;
}