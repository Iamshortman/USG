#include "EntityHealth.hpp"

#include "Common/Entity/Entity.hpp"

EntityHealth::EntityHealth(Entity* entity, double starting_health)
	:ComponentEntity(entity)
{
	this->health = starting_health;
}

void EntityHealth::applyDamage(double amount)
{
	this->health -= amount;

	if (this->health <= 0)
	{
		this->parent_entity->kill();
	}
}

void EntityHealth::applyHealing(double amount)
{
	this->health += amount;
}
