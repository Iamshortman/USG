#include "Client/Rendering/Model.hpp"

#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/TexturePool.hpp"

Model::Model()
{

}

Model::~Model()
{
	if (this->mesh != nullptr)
	{
		MeshPool::instance->releaseUsing(this->meshString);
	}

	if (this->program != nullptr)
	{
		ShaderPool::instance->releaseUsing(this->programString);
	}

	if (this->lightingProgram != nullptr)
	{
		ShaderPool::instance->releaseUsing(this->lightingProgramString);
	}

	if (this->controller != nullptr)
	{
		delete this->controller;
	}

	for (int i = 0; i < 10; i++)
	{
		if (this->textures[i] != 0)
		{
			TexturePool::instance->releaseUsing(this->texturesString[i]);
		}
	}
}

void Model::setMesh(std::string name)
{
	if (this->mesh != nullptr)
	{
		MeshPool::instance->releaseUsing(this->meshString);
	}

	this->meshString = name;
	MeshPool::instance->setUsing(this->meshString);
	this->mesh = MeshPool::instance->getModel(this->meshString);
}

void Model::setShader(std::string name)
{
	if (this->program != nullptr)
	{
		ShaderPool::instance->releaseUsing(this->programString);
	}

	this->programString = name;
	ShaderPool::instance->setUsing(this->programString);
	this->program = ShaderPool::instance->getShader(this->programString);
}

void Model::setLightingShader(std::string name)
{
	if (this->lightingProgram != nullptr)
	{
		ShaderPool::instance->releaseUsing(this->lightingProgramString);
	}

	this->lightingProgramString = name;
	ShaderPool::instance->setUsing(this->lightingProgramString);
	this->lightingProgram = ShaderPool::instance->getShader(this->lightingProgramString);
}

void Model::setController(RenderController* renderController)
{
	if (this->controller != nullptr)
	{
		delete this->controller;
	}

	this->controller = renderController;
}

//Need to work on this
//I don't like saving the name, rather save the int location.
void Model::addTexture(std::string name, int slot)
{
	if (this->textures[slot] != 0)
	{
		TexturePool::instance->releaseUsing(this->texturesString[slot]);
	}

	TexturePool::instance->setUsing(name);
	this->texturesString[slot] = name;
	this->textures[slot] = TexturePool::instance->getTexture(name);
}

Mesh* Model::getMesh()
{
	return this->mesh;
}

ShaderProgram* Model::getShader()
{
	return this->program;
}

ShaderProgram* Model::getLightingShader()
{
	return this->lightingProgram;
}

RenderController* Model::getController()
{
	return this->controller;
}

GLuint Model::getTexture(int slot)
{
	return this->textures[slot];
}