#ifndef ENTITY_DESTROYER_HPP
#define ENTITY_DESTROYER_HPP

#include "Common/EntityX_Include.hpp"

#include <queue>
class EntityDestroyer
{
public:
	static EntityDestroyer* getInstance();

	static void destroyEntity(Entity entity);

	void update(EntityX& ecs);

protected:
	//Instance for the Singleton design pattern;
	static EntityDestroyer* instance;
	EntityDestroyer() {};

	std::queue<Entity> entities_to_destroy;
};

#endif //ENTITY_DESTROYER_HPP