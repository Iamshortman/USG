#ifndef COMPONENT_LIGHT_HPP
#define COMPONENT_LIGHT_HPP 

#include "Common/Entity/Component.hpp"
#include "Common/Rendering/Lights.hpp"
#include "Common/Transform.hpp"


class ComponentLight : public Component
{
public:
	static const ComponentType Type = ComponentType::LIGHT;

	ComponentLight(float range, vector3F attenuation, vector3F color, float intensity); //Point Light
	ComponentLight(vector3F direction, float cutoff, float range, vector3F attenuation, vector3F color, float intensity); //Spot Light

	virtual ~ComponentLight();

	virtual void enable() override;
	virtual void disable() override;

	virtual void addtoEntity(EntityNode* entity);

	virtual ComponentType getComponentType() const override { return Type; };

	void setCastShadows(bool cast_shadows) { this->cast_shadows = cast_shadows; };
	bool getCastShadows() { return this->cast_shadows; };

	void setLightOn(bool light_on) { this->light_on = light_on; };
	bool getLightOn() { return this->light_on; };

	void setPointLight(float range, vector3F attenuation, vector3F color, float intensity);
	void setSpotLight(vector3F direction, float cutoff, float range, vector3F attenuation, vector3F color, float intensity);

	vector3D getLightPostion();
	matrix4 getLightSpaceMatrix();
	Transform getLightTransform();

	LightType getLightType();
	BaseLight* getLight();

protected:

	LightType light_type;
	BaseLight* light = nullptr;

	bool cast_shadows = true;
	bool light_on = true;

};

#endif //COMPONENT_LIGHT_HPP