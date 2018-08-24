#ifndef SHIP_CONTROLLER_HPP
#define SHIP_CONTROLLER_HPP

#include "Common/Component/ProjectileLauncher.hpp"
#include "Common/Component/ComponentEntity.hpp"

#include <set>

class ShipController: public ComponentEntity
{
public:
	ShipController(Entity* entity);
	~ShipController();

	virtual void update(double delta_time);

	void addProjectileLauncher(ProjectileLauncher* launcher);
	void removeProjectileLauncher(ProjectileLauncher* launcher);

	void addThruster(void* thruster);

	//Input Section
	bool fire_weapon = false;

protected:
	std::set<ProjectileLauncher*> guns;



};

#endif //SHIP_CONTROLLER_HPP