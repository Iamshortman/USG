#ifndef MODEL_HPP
#define MODEL_HPP 

#include "Common/Component/ComponentNode.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/Node.hpp"

#include "Common/Transform.hpp"
#include "Common/Types.hpp"

class Model : public ComponentNode
{
public:
	Model(I_Node* node, string mesh, string texture, string ambient_shader, string shadow_shader);
	virtual ~Model();

	string getMesh() const { return this->mesh_file_path; };
	string getTexture() const { return this->texture_name; };

	string getAmbientShader() const { return this->ambient_shader_name; };
	string getShadowShader() const { return this->shadow_shader_name; };

	void setCastShadows(bool cast_shadow) { this->cast_shadows = cast_shadow; };
	bool castShadows() const { return this->cast_shadows; };

protected:
	bool cast_shadows = true;

	string mesh_file_path;
	string texture_name;

	string ambient_shader_name;
	string shadow_shader_name;
};

#endif //MODEL_HPP