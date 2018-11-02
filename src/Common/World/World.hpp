#ifndef WORLD_HPP
#define WORLD_HPP

//Prototype Class
class World;

#include <vector>
#include <set>
#include <algorithm>

#include "Common/Entity/Entity.hpp"

typedef uint32_t WorldId;

class World
{
public:
	//TODO Figure out where better to put this
	vector3F ambientLight = vector3F(0.4f);

	const WorldId worldId;
	World(WorldId id);
	virtual ~World();
	virtual void update(double deltaTime);

	void addEntityToWorld(Entity* entity);
	void removeEntityFromWorld(Entity* entity);
	std::vector<Entity*>& getEntitiesInWorld() { return this->entitiesInWorld; };

	void setParent(Entity* entity);
	Entity* getParent();

	bool hasParentWorld();
	World* getParentWorld();

	Transform getWorldOffset();

protected:
	Entity* parent = nullptr;
	World* parentWorld = nullptr;

	std::vector<Entity*> entitiesInWorld;
};

#endif //WORLD_HPP