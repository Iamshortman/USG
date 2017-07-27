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

	worlds[id]->directionalLights.push_back(light);
}

void LightManager::addPointLight(WorldId id, PointLight* light)
{
	if (!worlds.count(id))
	{
		worlds[id] = new LightSet();
	}

	worlds[id]->pointLights.push_back(light);
}

void LightManager::addSpotLight(WorldId id, SpotLight* light)
{
	if (!worlds.count(id))
	{
		worlds[id] = new LightSet();
	}

	worlds[id]->spotLights.push_back(light);
}

void LightManager::removeDirectionalLight(WorldId id, DirectionalLight* light)
{
	std::vector<DirectionalLight*> vec = worlds[id]->directionalLights;
	vec.erase(std::remove(vec.begin(), vec.end(), light), vec.end());
}

void LightManager::removePointLight(WorldId id, PointLight* light)
{
	std::vector<PointLight*> vec = worlds[id]->pointLights;
	vec.erase(std::remove(vec.begin(), vec.end(), light), vec.end());
}

void LightManager::removeSpotLight(WorldId id, SpotLight* light)
{
	std::vector<SpotLight*> vec = worlds[id]->spotLights;
	vec.erase(std::remove(vec.begin(), vec.end(), light), vec.end());
}


void LightManager::addLight(WorldId id, LightType type, BaseLight* light)
{
	LightSet* set = this->getLightsForWorld(id);

	if (set == nullptr)
	{
		set = new LightSet();
		this->worlds[id] = set;
	}

	switch (type)
	{
	case Directional:
		this->addDirectionalLight(id, (DirectionalLight*)light);
		break;
	case Point:
		this->addPointLight(id, (PointLight*)light);
		break;
	case Spot:
		this->addSpotLight(id, (SpotLight*)light);
		break;
	}

}

void LightManager::removeLight(WorldId id, LightType type, BaseLight* light)
{
	if (this->getLightsForWorld(id) != nullptr)
	{
		switch (type)
		{
		case Directional:
			this->removeDirectionalLight(id, (DirectionalLight*)light);
			break;
		case Point:
			this->removePointLight(id, (PointLight*)light);
			break;
		case Spot:
			this->removeSpotLight(id, (SpotLight*)light);
			break;
		}
	}
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