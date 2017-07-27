#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP

#include <vector>
#include <hash_map>
#include <algorithm>
#include "Common/World/World.hpp"
#include "Client/Rendering/Lights.hpp"

struct LightSet
{
	std::vector<DirectionalLight*> directionalLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;
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

	void removeDirectionalLight(WorldId id, DirectionalLight* light);
	void removePointLight(WorldId id, PointLight* light);
	void removeSpotLight(WorldId id, SpotLight* light);

	void addLight(WorldId id, LightType type, BaseLight* light);
	void removeLight(WorldId id, LightType type, BaseLight* light);

	LightSet* getLightsForWorld(WorldId id);

	void destroyWorld(WorldId id);

private:
	std::hash_map<WorldId, LightSet*> worlds;
};

#endif //LIGHTMANAGER_HPP