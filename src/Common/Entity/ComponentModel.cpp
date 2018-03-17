#include "Common/Entity/ComponentModel.hpp"

#include "Common/Entity/EntityNode.hpp"

#ifdef CLIENT
#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"
#endif

ComponentModel::ComponentModel(string mesh, string shader, string lighting_shader, string texture)
{
	this->mesh_file_path = mesh;
	this->shader_name = shader;
	this->lighting_shader_name = lighting_shader;
	this->texture_name = texture;
}

ComponentModel::~ComponentModel()
{
	this->disable();
}

void ComponentModel::enable()
{
	if (!this->enabled && this->parent_entity != nullptr)
	{
		if (this->parent_entity->models.find(this) == this->parent_entity->models.end())
		{
			this->parent_entity->models.insert(this);
		}

#ifdef CLIENT
		MeshPool::instance->setUsing(this->mesh_file_path);
		ShaderPool::instance->setUsing(this->shader_name);
		ShaderPool::instance->setUsing(this->lighting_shader_name);
		TexturePool::instance->setUsing(this->texture_name);
#endif
		Component::enable();
	}
}

void ComponentModel::disable()
{
	if (this->enabled && this->parent_entity != nullptr)
	{
		if (this->parent_entity->models.find(this) != this->parent_entity->models.end())
		{
			this->parent_entity->models.erase(this);
		}

#ifdef CLIENT
		MeshPool::instance->releaseUsing(this->mesh_file_path);
		ShaderPool::instance->releaseUsing(this->shader_name);
		ShaderPool::instance->releaseUsing(this->lighting_shader_name);
		TexturePool::instance->releaseUsing(this->texture_name);
#endif
		Component::disable();
	}
}

void ComponentModel::addtoEntity(EntityNode* entity)
{
	this->disable();

	Component::addtoEntity(entity);

	this->enable();
}

ComponentType ComponentModel::getComponentType() const
{
	return ComponentType::MODEL;
}
