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
	shader.program = nullptr;
	shader.usingCount = 0;
	shader.vertexPath = vertex;
	shader.fragmentPath = fragment;
	shader.attributeLocation = attributeLocation;

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
	
	if (this->shaders[name].usingCount <= 0)
	{
		this->shaders[name].program = new ShaderProgram(this->shaders[name].vertexPath, this->shaders[name].fragmentPath, this->shaders[name].attributeLocation);
	}

	this->shaders[name].usingCount++;
}

unsigned short ShaderPool::getUsing(string name)
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

	if (this->shaders[name].usingCount > 0)
	{
		this->shaders[name].usingCount--;
	}

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
		this->shaders[name].program = nullptr;
	}
}

ShaderProgram* ShaderPool::getShader(string name)
{
	//Does not contain
	if (!this->shaders.count(name))
	{
		return nullptr;
	}

	if (shaders[name].usingCount <= 0)
	{
		printf("Error: Shader %s not loaded\n", name.c_str());
	}

	return shaders[name].program;
}
