#include "ComponentLight.hpp"
#include "Common/Entity/EntityNode.hpp"

ComponentLight::ComponentLight(float range, vector3F attenuation, vector3F color, float intensity)
{
	this->setPointLight(range, attenuation, color, intensity);
}

ComponentLight::ComponentLight(vector3F direction, float cutoff, float range, vector3F attenuation, vector3F color, float intensity)
{
	this->setSpotLight(direction, cutoff, range, attenuation, color, intensity);
}

ComponentLight::~ComponentLight()
{
	this->disable();

	if (this->light != nullptr)
	{
		delete this->light;
	}
}

void ComponentLight::enable()
{
	if (!this->enabled && this->parent_entity != nullptr)
	{
		//this->parent_entity->addLight(this);

		Component::enable();
	}
}

void ComponentLight::disable()
{
	if (this->enabled && this->parent_entity != nullptr)
	{
		//this->parent_entity->removeLight(this);

		Component::disable();
	}
}

void ComponentLight::addtoEntity(EntityNode * entity)
{
	this->disable();

	Component::addtoEntity(entity);

	this->enable();
}

void ComponentLight::setPointLight(float range, vector3F attenuation, vector3F color, float intensity)
{
	if (this->light != nullptr)
	{
		delete this->light;
	}

	this->light_type = LightType::Point;
	this->light = new PointLight(range, attenuation, color, intensity);
}

void ComponentLight::setSpotLight(vector3F direction, float cutoff, float range, vector3F attenuation, vector3F color, float intensity)
{
	if (this->light != nullptr)
	{
		delete this->light;
	}

	this->light_type = LightType::Spot;
	this->light = new SpotLight(direction, cutoff, range, attenuation, color, intensity);
}
