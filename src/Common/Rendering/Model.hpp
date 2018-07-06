#ifndef COMPONENT_MODEL_HPP
#define COMPONENT_MODEL_HPP 

#include <string>
using std::string;

class Model
{
public:
	Model(string mesh, string texture, string gbuffer_shader, string shadow_shader);
	virtual ~Model();

	inline string getMesh() const { return this->mesh_file_path; };
	inline string getTexture() const { return this->texture_name; };

	inline string getGBufferShader() const { return this->gbuffer_shader_name; };
	inline string getShadowShader() const { return this->shadow_shader_name; };

	inline void setCastShadows(bool cast_shadow) { this->cast_shadows = cast_shadow; };
	inline bool castShadows() const { return this->cast_shadows; };

protected:
	bool cast_shadows = true;

	string mesh_file_path;
	string texture_name;

	string gbuffer_shader_name;
	string shadow_shader_name;
};

#endif //COMPONENT_MODEL_HPP