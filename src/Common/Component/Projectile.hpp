#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Common/Component/ComponentEntity.hpp"

#include "Common/Transform.hpp"

class Projectile : public ComponentEntity
{
public:
	Projectile(Entity* entity, vector3D velocity);

	virtual void update(double delta_time);
private:
	vector3D velocity;
};
#endif //PROJECTILE_HPP