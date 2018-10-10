#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#include "Common/EntityX_Include.hpp"
#include "Common/Physics/ReactPhysics3D_Include.hpp"
#include "Common/Physics/RigidBody.hpp"

class SingleRayCallback: public reactphysics3d::RaycastCallback
{
public:
	bool has_hit = false;

	reactphysics3d::decimal smallest_hit_fraction = 1.1;
	reactphysics3d::CollisionBody* hit_body = nullptr;
	reactphysics3d::ProxyShape* hit_shape = nullptr;
	reactphysics3d::Vector3 world_point;
	reactphysics3d::Vector3 world_normal;

	reactphysics3d::decimal notifyRaycastHit(const  reactphysics3d::RaycastInfo& raycastInfo)
	{
		if (raycastInfo.hitFraction < this->smallest_hit_fraction)
		{
			this->has_hit = true;
			this->hit_body = raycastInfo.body;
			this->hit_shape = raycastInfo.proxyShape;
			this->world_point = raycastInfo.worldPoint;
			this->world_normal = raycastInfo.worldNormal;
			this->smallest_hit_fraction = raycastInfo.hitFraction;
		}

		return 1.0;
	};
};


struct SingleRaycastResult
{
	bool has_hit = false;

	Entity hit_entity;
	Entity hit_entity_root;

	vector3D world_point;
	vector3D world_normal;
};

class PhysicsWorld
{

public:
	PhysicsWorld();
	virtual ~PhysicsWorld();

	void update(double delta_time);

	SingleRaycastResult SingleRaycast(vector3D& start, vector3D& end, EntityManager& es);

	void addRigidBody(RigidBody* rigid_body, Transform transform);
	void removeRigidBody(RigidBody* rigid_body);

protected:
	std::set<RigidBody*> rigid_bodies;
	reactphysics3d::DynamicsWorld* dynamics_world = nullptr;
};

#endif //PHYSICS_WORLD_HPP