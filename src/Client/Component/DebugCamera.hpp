#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include "Common/EntityX_Include.hpp"

struct DebugCamera
{
	DebugCamera(double linear, double angular) : linear_speed(linear), angular_speed(angular) {};

	//meter per second
	double linear_speed;

	//rad per second
	double angular_speed;
};

class DebugCameraSystem : public System<DebugCameraSystem>
{
public:
	void update(EntityManager &es, EventManager &events, TimeDelta delta_time) override;
};

#endif //PLAYERCONTROL_HPP
