#ifndef PROJECTILE_LAUNCHER_HPP
#define PROJECTILE_LAUNCHER_HPP

#include "Common/Component/ComponentNode.hpp"

class ProjectileLauncher : public ComponentNode
{
public:
	ProjectileLauncher(I_Node* node);

	virtual void update(double delta_time);

};

#endif // !PROJECTILE_LAUNCHER_HPP
