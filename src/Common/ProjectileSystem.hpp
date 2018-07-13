#ifndef PROJECTILE_SYSTEM_HPP
#define PROJECTILE_SYSTEM_HPP

#include "Common/EntityX_Include.hpp"
#include "Common/Transform.hpp"

struct ProjectileLauncher
{

};

struct Projectile
{
	vector3D velocity;
};


class ProjectileSystem : public System<ProjectileSystem>
{
public:
	void update(EntityManager &es, EventManager &events, TimeDelta delta_time) override;
};

#endif //PROJECTILE_SYSTEM_HPP