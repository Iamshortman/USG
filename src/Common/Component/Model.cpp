#include "Model.hpp"

#ifdef CLIENT
#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"
#endif

Model::Model(I_Node* node, string mesh, string texture, string ambient_shader, string shadow_shader)
	:ComponentNode(node)
{
	this->mesh_file_path = mesh;
	this->texture_name = texture;

	this->ambient_shader_name = ambient_shader;
	this->shadow_shader_name = shadow_shader;

#ifdef CLIENT
	MeshPool::getInstance()->setUsing(this->mesh_file_path);
	TexturePool::getInstance()->setUsing(this->texture_name);

	ShaderPool::getInstance()->setUsing(this->ambient_shader_name);
	//ShaderPool::getInstance()->setUsing(this->shadow_shader_name);
#endif
}

Model::~Model()
{
#ifdef CLIENT
	MeshPool::getInstance()->releaseUsing(this->mesh_file_path);
	TexturePool::getInstance()->releaseUsing(this->texture_name);

	ShaderPool::getInstance()->releaseUsing(this->ambient_shader_name);
	//ShaderPool::getInstance()->releaseUsing(this->shadow_shader_name);
#endif
}
