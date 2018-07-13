#ifndef TIMETOLIVE_HPP
#define TIMETOLIVE_HPP

#include "Common/EntityX_Include.hpp"

struct TimeToLive
{
	TimeToLive(double lifeTime) : timeToLive(lifeTime) {};

	//The Time left in seconds
	double timeToLive;
};

class TimeToLiveSystem : public System<TimeToLiveSystem>
{
public:
	void update(EntityManager& es, EventManager& events, TimeDelta delta_time) override
	{
		ComponentHandle<TimeToLive> time_pattern;
		for (Entity entity : es.entities_with_components(time_pattern))
		{
			ComponentHandle<TimeToLive> time = entity.component<TimeToLive>();
			time->timeToLive -= delta_time;

			if (time->timeToLive <= 0)
			{
				entity.destroy();
			}
		}
	};
};

#endif //TIMETOLIVE_HPP