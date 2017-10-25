#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <stack>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Common/Entity/Entity.hpp"

struct Creator
{
	virtual Entity* create(EntityId id) = 0;
};

class EntityManager
{

public:
	//Instance for the Singleton design pattern;
	static EntityManager* instance;

	EntityManager();

	void update();

	EntityId getNextId();

	Entity* createEntity(ENTITYTYPE type);
	Entity* createEntity(ENTITYTYPE type, EntityId entityId);
	Entity* createEntityFromNetwork(BitStream* in);

	void destroyEntity(EntityId id);

	Entity* getEntity(EntityId id);

	std::unordered_map<EntityId, Entity*>::iterator getAllEntities();
	std::unordered_map<EntityId, Entity*> entities;

	void registerCreator(ENTITYTYPE type, Creator* creator);
private:
	std::unordered_map<ENTITYTYPE, Creator*> creators;
	std::stack<EntityId> entities_to_delete;

	EntityId nextId = 1;

};

#endif //ENTITYMANAGER_HPP