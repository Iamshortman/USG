#include "Model.hpp"

#ifdef CLIENT
#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"
#endif

Model::Model(string mesh, string texture, string gbuffer_shader, string shadow_shader)
{
	this->mesh_file_path = mesh;
	this->texture_name = texture;

	this->gbuffer_shader_name = gbuffer_shader;
	this->shadow_shader_name = shadow_shader;

#ifdef CLIENT
	MeshPool::getInstance()->setUsing(this->mesh_file_path);
	TexturePool::getInstance()->setUsing(this->texture_name);

	ShaderPool::getInstance()->setUsing(this->gbuffer_shader_name);
	ShaderPool::getInstance()->setUsing(this->shadow_shader_name);
#endif
}

Model::~Model()
{
#ifdef CLIENT
		MeshPool::getInstance()->releaseUsing(this->mesh_file_path);
		TexturePool::getInstance()->releaseUsing(this->texture_name);

		ShaderPool::getInstance()->releaseUsing(this->gbuffer_shader_name);
		ShaderPool::getInstance()->releaseUsing(this->shadow_shader_name);
#endif
}
