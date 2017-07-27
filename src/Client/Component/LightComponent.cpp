#include "Client/Component/LightComponent.hpp"

#include "Client/Rendering/LightManager.hpp"

LightComponent::LightComponent(LightType type, BaseLight* light)
{
	this->lightType = type;
	this->light = light;
}

void LightComponent::addToWorld(World* world)
{
	LightManager::instance->addLight(world->worldId, this->lightType, this->light);
}

void LightComponent::removeFromWorld(World* world)
{
	LightManager::instance->removeLight(world->worldId, this->lightType, this->light);
}

LightType LightComponent::getLightType()
{
	return this->lightType;
}

BaseLight* LightComponent::getLight()
{
	return this->light;
}