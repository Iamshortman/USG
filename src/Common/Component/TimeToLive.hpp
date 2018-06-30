#ifndef TIMETOLIVE_HPP
#define TIMETOLIVE_HPP

#include "Common/Component/ComponentEntity.hpp"
#include "Common/Entity/Entity.hpp"

struct TimeToLive: public ComponentEntity
{
	//The Time left in seconds
	double m_timeToLive;

	TimeToLive(Entity* entity, double lifeTime)
		:ComponentEntity(entity)
	{
		m_timeToLive = lifeTime;
	};

	virtual void update(double deltaTime) 
	{
		m_timeToLive -= deltaTime;
		if (m_timeToLive <= 0.0)
		{
			this->parent_entity->kill();
		}
	};
};
#endif //TIMETOLIVE_HPP