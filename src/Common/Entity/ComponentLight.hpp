#ifndef COMPONENT_LIGHT_HPP
#define COMPONENT_LIGHT_HPP 

#include "Common/Entity/Component.hpp"
#include "Common/Rendering/Lights.hpp"



class ComponentLight : public Component
{
public:
	static const ComponentType Type = ComponentType::LIGHT;

	ComponentLight();
	virtual ~ComponentLight();

	virtual void enable() override;
	virtual void disable() override;

	virtual void addtoEntity(EntityNode* entity);

	virtual ComponentType getComponentType() const override { return Type; };

	void setCastShadows(bool cast_shadows) { this->cast_shadows = cast_shadows; };
	bool getCastShadows() { return this->cast_shadows; };

	void setLightOn(bool light_on) { this->light_on = light_on; };
	bool getLightOn() { return this->light_on; };

protected:


	PointLight point_light;
	SpotLight spot_light;

	bool cast_shadows;
	bool light_on;

};

#endif //COMPONENT_LIGHT_HPP