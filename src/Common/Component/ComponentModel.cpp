#include "ComponentModel.hpp"

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
	if (this->temp_mesh != nullptr)
	{
		delete this->temp_mesh;
	}
	this->disable();
}

void ComponentModel::enable()
{
#ifdef CLIENT
	if (!this->enabled)
	{
		MeshPool::instance->setUsing(this->mesh_file_path);
		TexturePool::instance->setUsing(this->texture_name);

		ShaderPool::instance->setUsing(this->ambient_shader_name);
		ShaderPool::instance->setUsing(this->lighting_shader_name);
		ShaderPool::instance->setUsing(this->shadow_shader_name);
	}
#endif
	Component::enable();
}

void ComponentModel::disable()
{
#ifdef CLIENT
	if (this->enabled)
	{
		MeshPool::instance->releaseUsing(this->mesh_file_path);
		TexturePool::instance->releaseUsing(this->texture_name);

		ShaderPool::instance->releaseUsing(this->ambient_shader_name);
		ShaderPool::instance->releaseUsing(this->lighting_shader_name);
		ShaderPool::instance->releaseUsing(this->shadow_shader_name);
	}
#endif
	Component::disable();
}
