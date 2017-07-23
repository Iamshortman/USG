#include "Client/Resource/ShaderPool.hpp"

ShaderPool* ShaderPool::instance;

ShaderPool::ShaderPool()
{
	instance = this;
}

ShaderPool::~ShaderPool()
{
	for (auto it = this->shaders.begin(); it != this->shaders.end(); ++it)
	{
		delete it->second.program;
	}
}

void ShaderPool::loadShader(string name, string vertex, string fragment, vector<AttributeLocation> attributeLocation)
{
	//Does contain
	if (this->shaders.count(name))
	{
		//Already loaded
		return;
	}

	ShaderResource shader;
	shader.program = new ShaderProgram(vertex, fragment, attributeLocation);
	shader.usingCount = 0;

	this->shaders[name] = shader;
}

void ShaderPool::setUsing(string name)
{
	//TODO, load the shader if it doesn't exsist
	//Does not contain
	if (!this->shaders.count(name))
	{
		//TODO load here
		return;
	}
	
	this->shaders[name].usingCount++;
}

int ShaderPool::getUsing(string name)
{
	//Return -1 if it is not loaded
	//Does not contain
	if (!this->shaders.count(name))
	{
		return -1;
	}

	return this->shaders[name].usingCount;
}

void ShaderPool::releaseUsing(string name)
{
	//Does not contain
	if (!this->shaders.count(name))
	{
		return;
	}

	this->shaders[name].usingCount--;

	//If no one is using it, unload it
	if (this->shaders[name].usingCount <= 0)
	{
		this->unloadShader(name);
	}
}

void ShaderPool::unloadShader(string name)
{
	//Does contain
	if (this->shaders.count(name))
	{
		delete this->shaders[name].program;
		this->shaders.erase(name);
	}
}

ShaderProgram* ShaderPool::getShader(string name)
{
	//Does not contain
	if (!this->shaders.count(name))
	{
		return nullptr;
	}

	return shaders[name].program;
}
