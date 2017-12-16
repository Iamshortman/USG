#include "Planet.hpp"

Planet::Planet(World* world, vector3D postion, double mass)
{
	this->parent = world;
	this->postion = postion;
	this->mass = mass;
}

Planet::~Planet()
{

}

void Planet::update(double deltaTime)
{
	auto entitySet = this->parent->getEntitiesInWorld();

	for(auto it = entitySet->begin(); it != entitySet->end(); it++)
	{
		Entity* entity = (*it);
		if (entity->getRigidBody() == nullptr || entity->getEntityType() == ENTITYTYPE::PLAYER_THIS || entity->getEntityType() == ENTITYTYPE::PLAYER_OTHER)
		{
			continue;
		}

		vector3D entityPos = entity->getTransform().getPosition();
		double distance2 = glm::distance2(this->postion, entityPos);
		
		double force = 0.0;
		if (distance2 != 0.0 && distance2 > 1.0)
		{
			force = (UNIVERSAL_GRAVITY_CONST * this->mass * entity->getRigidBody()->getMass()) / distance2;
		}
		//printf("Force: %lf\n", force);
		//force *= deltaTime;

		vector3D direction = glm::normalize(this->postion - entityPos);

		entity->getRigidBody()->Activate(true);
		entity->getRigidBody()->applyCentralImpulse(direction * force);
	}
}