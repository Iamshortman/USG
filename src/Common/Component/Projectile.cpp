#include "Projectile.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/World/World.hpp"

#include "Common/Component/EntityHealth.hpp"

#include "Common/Logger/Logger.hpp"

Projectile::Projectile(Entity* entity, vector3D velocity)
	:ComponentEntity(entity)
{
	this->velocity = velocity;
}

void Projectile::update(double delta_time)
{
	World* world = this->parent_entity->getWorld();

	if (world != nullptr)
	{
		Transform transform = this->parent_entity->getLocalTransform();
		vector3D start_pos = transform.getPosition();
		vector3D end_pos = start_pos +  (this->velocity * delta_time);

		SingleRayTestResult result = world->singleRayTest(start_pos, end_pos);

		//Update Projectile Position
		transform.setPosition(end_pos);
		this->parent_entity->setLocalTransform(transform);

		if (result.hasHit == true)
		{
			this->parent_entity->kill();

			if (result.entity->hasComponent<EntityHealth>())
			{
				EntityHealth* health = result.entity->getComponent<EntityHealth>();
				health->applyDamage(10.0);
			}
		}
	}
}
