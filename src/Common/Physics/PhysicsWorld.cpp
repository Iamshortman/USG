#include "PhysicsWorld.hpp"

PhysicsWorld::PhysicsWorld()
{
	this->dynamics_world = new reactphysics3d::DynamicsWorld(reactphysics3d::Vector3(0.0, 0.0, 0.0));
}

PhysicsWorld::~PhysicsWorld()
{
	std::vector<RigidBody*> bodies;
	for (RigidBody* rigid_body : this->rigid_bodies)
	{
		bodies.push_back(rigid_body);
	}

	//Can't remove from a set while inside it
	//This is the primary reason I want to write a set class implimentated by a vector
	for(int i = 0; i < bodies.size(); i++)
	{
		bodies[i]->destroyRigidBody();
	}

	delete this->dynamics_world;
}

void PhysicsWorld::update(double delta_time)
{
	this->dynamics_world->update(delta_time);
}

SingleRaycastResult PhysicsWorld::SingleRaycast(vector3D& start, vector3D& end, EntityManager& es)
{
	SingleRaycastResult result;

	reactphysics3d::Ray ray = reactphysics3d::Ray(toRecVec3(start), toRecVec3(end));
	SingleRayCallback callback;
	this->dynamics_world->raycast(ray, &callback);

	if (callback.has_hit)
	{
		result.has_hit = true;

		result.hit_entity = es.get(*((Entity::Id*)callback.hit_shape->getUserData()));
		result.hit_entity_root = es.get(*((Entity::Id*)callback.hit_body->getUserData()));

		result.world_point = toGlmVec3(callback.world_point);
		result.world_normal = toGlmVec3(callback.world_normal);
	}

	return result;
}

void PhysicsWorld::addRigidBody(RigidBody* rigid_body, Transform transform)
{
	this->rigid_bodies.insert(rigid_body);
	rigid_body->rigid_body = this->dynamics_world->createRigidBody(toRecTrans(transform));
}

void PhysicsWorld::removeRigidBody(RigidBody* rigid_body)
{
	this->rigid_bodies.erase(rigid_body);
	this->dynamics_world->destroyRigidBody(rigid_body->rigid_body);
	rigid_body->rigid_body = nullptr;
}
