#ifndef COMPONENT_MODEL_HPP
#define COMPONENT_MODEL_HPP 

#include "Common/Types.hpp"

class ComponentModel
{
public:
	ComponentModel(string mesh, string texture, string ambient_shader, string lighting_shader, string shadow_shader);
	virtual ~ComponentModel();

	string getMesh() const { return this->mesh_file_path; };
	string getTexture() const { return this->texture_name; };

	string getAmbientShader() const { return this->ambient_shader_name; };
	string getLightingShader() const { return this->lighting_shader_name; };
	string getShadowShader() const { return this->shadow_shader_name; };

	void setCastShadows(bool cast_shadow) { this->cast_shadows = cast_shadow; };
	bool castShadows() const { return this->cast_shadows; };

protected:
	bool cast_shadows = true;

	string mesh_file_path;
	string texture_name;

	string ambient_shader_name;
	string lighting_shader_name;
	string shadow_shader_name;
};

#endif //COMPONENT_MODEL_HPP