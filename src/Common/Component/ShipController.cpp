#include "ShipController.hpp"

ShipController::ShipController(Entity* entity)
	:ComponentEntity(entity)
{
}

ShipController::~ShipController()
{
}

void ShipController::update(double delta_time)
{
	if (fire_weapon)
	{
		for (auto launcher : this->guns)
		{
			launcher->fire_weapon = true;
		}
	}
}

void ShipController::addProjectileLauncher(ProjectileLauncher* launcher)
{
	this->guns.insert(launcher);
}

void ShipController::removeProjectileLauncher(ProjectileLauncher * launcher)
{
	this->guns.erase(launcher);
}

void ShipController::addThruster(void* thruster)
{
}
