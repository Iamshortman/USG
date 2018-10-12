#include "EntityDestroyer.hpp"

EntityDestroyer* EntityDestroyer::instance = nullptr;
inline EntityDestroyer * EntityDestroyer::getInstance()
{
	if (instance == nullptr)
	{
		instance = new EntityDestroyer();
	}

	return instance;
}

void EntityDestroyer::destroyEntity(Entity entity)
{
	getInstance()->entities_to_destroy.push(entity);
}

void EntityDestroyer::update(EntityX & ecs)
{
	while (!this->entities_to_destroy.empty())
	{
		Entity entity = this->entities_to_destroy.front();
		this->entities_to_destroy.pop();
		entity.destroy();
	}
}
