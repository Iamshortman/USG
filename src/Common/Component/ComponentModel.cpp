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

#ifdef CLIENT
	MeshPool::getInstance()->setUsing(this->mesh_file_path);
	TexturePool::getInstance()->setUsing(this->texture_name);

	ShaderPool::getInstance()->setUsing(this->ambient_shader_name);
	//ShaderPool::getInstance()->setUsing(this->lighting_shader_name);
	//ShaderPool::getInstance()->setUsing(this->shadow_shader_name);
#endif
}

ComponentModel::~ComponentModel()
{
#ifdef CLIENT
		MeshPool::getInstance()->releaseUsing(this->mesh_file_path);
		TexturePool::getInstance()->releaseUsing(this->texture_name);

		ShaderPool::getInstance()->releaseUsing(this->ambient_shader_name);
		//ShaderPool::getInstance()->releaseUsing(this->lighting_shader_name);
		//ShaderPool::getInstance()->releaseUsing(this->shadow_shader_name);
#endif
}
