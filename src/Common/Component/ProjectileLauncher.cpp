#include "ProjectileLauncher.hpp"

#include "Common/Entity/Node.hpp"
#include "Common/Entity/Entity.hpp"
#include "Common/World/World.hpp"

#include "Common/Entity/EntityManager.hpp"

#include "Common/Rendering/Model.hpp"
#include "Common/Component/TimeToLive.hpp"
#include "Common/Component/Projectile.hpp"

#include "Common/Component/ShipController.hpp"

ProjectileLauncher::ProjectileLauncher()
{
}

void ProjectileLauncher::update(double delta_time)
{
	const double time_shoot = 1.0 / 20.0;

	if (this->fire_weapon)
	{
		time_count += delta_time;

		if (time_count >= time_shoot)
		{
			double velocity = 1000.0;

			NodeEntity* entity = (NodeEntity*)this->parent_node->getEntity();
			Transform transform = this->parent_node->getWorldTransform();

			Entity* bullet = EntityManager::getInstance()->createEntity();
			bullet->addToWorld(entity->getWorld());
			bullet->setLocalTransform(transform);

			bullet->addNodeComponent<Model>("res/models/bullet.obj", "res/textures/White.png", "res/shaders/Textured_Glow", "res/shaders/Shadow");

			bullet->addComponent<TimeToLive>(3.0);
			bullet->addComponent<Projectile>((transform.getForward() * velocity) + entity->getRigidBody()->getLinearVelocity());
			
			time_count = 0.0;
		}
	}
	else
	{
		time_count = 0.0;
	}

	this->fire_weapon = false;
}

void ProjectileLauncher::addToEntity()
{
	Entity* parent = this->parent_node->getEntity();
	if (parent != nullptr && parent->hasComponent<ShipController>())
	{
		parent->getComponent<ShipController>()->addProjectileLauncher(this);
	}
}

void ProjectileLauncher::removeFromEntity()
{
	Entity* parent = this->parent_node->getEntity();
	if (parent != nullptr && parent->hasComponent<ShipController>())
	{
		parent->getComponent<ShipController>()->removeProjectileLauncher(this);
	}
}
