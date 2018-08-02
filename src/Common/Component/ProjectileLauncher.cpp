#include "ProjectileLauncher.hpp"

#include "Common/Entity/Node.hpp"
#include "Common/Entity/Entity.hpp"
#include "Common/World/World.hpp"

#include "Common/Entity/EntityManager.hpp"

#include "Common/Rendering/Model.hpp"
#include "Common/Component/TimeToLive.hpp"
#include "Common/Component/Projectile.hpp"

#ifdef CLIENT
#include "Client/Input/InputManager.hpp"
#endif // CLIENT

ProjectileLauncher::ProjectileLauncher()
{
}

void ProjectileLauncher::update(double delta_time)
{
	const double time_shoot = 1.0 / 5.0;
	static double time_count = 0.0;

	bool shoot = false;

#ifdef CLIENT
	shoot = InputManager::getInstance()->getButtonDown("Flight_Shoot");
#endif // CLIENT

	if (shoot)
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

			bullet->addNodeComponent<Model>("res/models/bullet.obj", "res/textures/Purple.png", "res/shaders/Textured", "");

			bullet->addComponent<TimeToLive>(1.0);
			bullet->addComponent<Projectile>((transform.getForward() * velocity) + entity->getRigidBody()->getLinearVelocity());
			
			time_count = 0.0;
		}
	}
	else
	{
		time_count = 0.0;
	}
}
