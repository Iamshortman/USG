#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <stack>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Common/Entity/Entity.hpp"
#include "Common/Types.hpp"

class EntityManager
{
public:
	static EntityManager* getInstance();

	void update();

	Entity* createEntity();
	Entity* getEntity(EntityId id);
	void destroyEntity(EntityId id);

private:
	//Instance for the Singleton design pattern;
	static EntityManager* instance;
	EntityManager();

	std::unordered_map<EntityId, Entity*> game_objects;
	std::stack<EntityId> game_objects_to_delete;

	EntityId getNextId();
	EntityId next_id = 1;
};

#endif //ENTITYMANAGER_HPP