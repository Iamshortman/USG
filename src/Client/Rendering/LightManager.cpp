#include "Client/Rendering/LightManager.hpp"

LightManager* LightManager::instance = nullptr;

LightManager::LightManager()
{
	instance = this;
}
LightManager::~LightManager()
{

}

void LightManager::addDirectionalLight(WorldId id, DirectionalLight* light)
{
	if (!worlds.count(id))
	{
		worlds[id] = new LightSet();
	}

	worlds[id]->directionalLights.insert(light);
}

void LightManager::addPointLight(WorldId id, PointLight* light)
{
	if (!worlds.count(id))
	{
		worlds[id] = new LightSet();
	}

	worlds[id]->pointLights.insert(light);
}

void LightManager::addSpotLight(WorldId id, SpotLight* light)
{
	if (!worlds.count(id))
	{
		worlds[id] = new LightSet();
	}

	worlds[id]->spotLights.insert(light);
}

LightSet* LightManager::getLightsForWorld(WorldId id)
{
	if (!worlds.count(id))
	{
		return nullptr;
	}

	return worlds[id];
}

void LightManager::destroyWorld(WorldId id)
{
	if (worlds.count(id))
	{
		delete worlds[id];
		worlds.erase(id);
	}
}