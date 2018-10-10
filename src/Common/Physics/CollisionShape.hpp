#ifndef COLLISION_SHAPE_HPP
#define COLLISION_SHAPE_HPP

#include "Common/Physics/ReactPhysics3D_Include.hpp"

struct CollisionShape
{
	CollisionShape(reactphysics3d::CollisionShape* shape, double mass = 1.0)
		:shape(shape), mass(mass){};

	~CollisionShape() 
	{ 
		if (this->proxy != nullptr)
		{
			this->proxy->getBody()->removeCollisionShape(this->proxy);
		}

		if (this->shape != nullptr)
		{
			delete this->shape;
		}
	};

	reactphysics3d::CollisionShape* shape;
	double mass;

	Transform last_relative_transform;
	reactphysics3d::ProxyShape* proxy = nullptr;
};

#endif //COLLISION_SHAPE_HPP