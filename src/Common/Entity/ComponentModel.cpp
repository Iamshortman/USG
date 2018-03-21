#include "Common/Entity/ComponentModel.hpp"

#include "Common/Entity/EntityNode.hpp"

#ifdef CLIENT
#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"
#endif

ComponentModel::ComponentModel(string mesh, string texture, string ambient_shader, string lighting_shader, string shadow_shader)
{
	this->mesh_file_path = mesh;
	this->texture_name = texture;

	this->ambient_shader_name = ambient_shader;
	this->lighting_shader_name = lighting_shader;
	this->shadow_shader_name = shadow_shader;
}

ComponentModel::~ComponentModel()
{
	this->disable();
}

void ComponentModel::enable()
{
	if (!this->enabled && this->parent_entity != nullptr)
	{
		this->parent_entity->addModel(this);

#ifdef CLIENT
		MeshPool::instance->setUsing(this->mesh_file_path);
		TexturePool::instance->setUsing(this->texture_name);

		ShaderPool::instance->setUsing(this->ambient_shader_name);
		ShaderPool::instance->setUsing(this->lighting_shader_name);
		ShaderPool::instance->setUsing(this->shadow_shader_name);
#endif
		Component::enable();
	}
}

void ComponentModel::disable()
{
	if (this->enabled && this->parent_entity != nullptr)
	{
		this->parent_entity->removeModel(this);

#ifdef CLIENT
		MeshPool::instance->releaseUsing(this->mesh_file_path);
		TexturePool::instance->releaseUsing(this->texture_name);

		ShaderPool::instance->releaseUsing(this->ambient_shader_name);
		ShaderPool::instance->releaseUsing(this->lighting_shader_name);
		ShaderPool::instance->releaseUsing(this->shadow_shader_name);
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
