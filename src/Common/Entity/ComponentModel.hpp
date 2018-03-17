#include "Common/Entity/Component.hpp"
#include "Common/Types.hpp"

class ComponentModel : public Component
{
public:
	ComponentModel(string mesh, string shader, string lighting_shader, string texture);
	virtual ~ComponentModel();

	string getMesh() const { return this->mesh_file_path; };
	string getShader() const { return this->shader_name; };
	string getLightingShader() const { return this->lighting_shader_name; };
	string getTexture() const { return this->texture_name; };


	virtual void enable() override;
	virtual void disable() override;

	virtual void addtoEntity(EntityNode* entity);

	virtual ComponentType getComponentType() const override;

protected:
	string mesh_file_path;
	string shader_name;
	string lighting_shader_name;
	string texture_name;
};