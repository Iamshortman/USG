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

struct WorldChange
{
	Entity* entity;
	World* newWorld;
	Transform newTrans;
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
	Entity* getEntity(EntityId id);
	void destroyEntity(EntityId id);

	void ChangeWorld(Entity* entity, World* newWorld, Transform newTrans = Transform());

	std::unordered_map<EntityId, Entity*>::iterator getAllEntities();
	std::unordered_map<EntityId, Entity*> entities;

	void registerCreator(ENTITYTYPE type, Creator* creator);
private:
	std::unordered_map<ENTITYTYPE, Creator*> creators;
	std::stack<EntityId> entities_to_delete;
	std::stack<WorldChange> entities_world_change;

	EntityId nextId = 1;

};

#endif //ENTITYMANAGER_HPP