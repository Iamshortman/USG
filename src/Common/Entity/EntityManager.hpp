#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Common/Entity/Entity.hpp"

class EntityManager
{

public:
	//Instance for the Singleton design pattern;
	static EntityManager* instance;

	EntityManager();

	Entity* createNewEntity();
	void destroyEntity(EntityId id);

	Entity* getEntity(EntityId id);
	std::unordered_map<EntityId, Entity*>::iterator getAllEntities();

private:
	std::unordered_map<EntityId, Entity*> entities;
	EntityId nextId = 1;

};

#endif //ENTITYMANAGER_HPP