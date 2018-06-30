#ifndef ENTITY_HEALTH_HPP
#define ENTITY_HEALTH_HPP

#include "Common/Component/ComponentEntity.hpp"

#include "Common/Types.hpp"

class EntityHealth : public ComponentEntity
{
public:
	EntityHealth(Entity* entity, double starting_health);

	inline double getHealth() { return this->health; };

	void applyDamage(double amount);
	void applyHealing(double amount);

private:
	double health;
};

#endif //ENTITY_HEALTH_HPP