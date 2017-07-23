#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>
#include <hash_map>
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
	std::hash_map<EntityId, Entity*>::iterator getAllEntities();

private:
	std::hash_map<EntityId, Entity*> entities;
	EntityId nextId = 1;

};

#endif //ENTITYMANAGER_HPP