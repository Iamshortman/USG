#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP

#include <set>
#include <hash_map>
#include <algorithm>
#include "Common/World/World.hpp"
#include "Client/Rendering/Lights.hpp"

struct LightSet
{
	std::set<DirectionalLight*> directionalLights;
	std::set<PointLight*> pointLights;
	std::set<SpotLight*> spotLights;
};

class LightManager
{

public:
	//Instance for the Singleton design pattern;
	static LightManager* instance;

	LightManager();
	~LightManager();

	void addDirectionalLight(WorldId id, DirectionalLight* light);
	void addPointLight(WorldId id, PointLight* light);
	void addSpotLight(WorldId id, SpotLight* light);

	LightSet* getLightsForWorld(WorldId id);

	void destroyWorld(WorldId id);

private:
	std::hash_map<WorldId, LightSet*> worlds;
};

#endif //LIGHTMANAGER_HPP