#include "ProjectileSystem.hpp"

#include "Common/Transforms.hpp"
#include "Common/Rendering/Model.hpp"
#include "Common/TimeToLiveSystem.hpp"

#ifdef CLIENT
#include "Client/Input/InputManager.hpp"
#endif // CLIENT

void ProjectileSystem::update(EntityManager& es, EventManager& events, TimeDelta delta_time)
{
	bool shoot = false;

#ifdef CLIENT
	shoot = InputManager::getInstance()->getButtonDown("Flight_Shoot");
#endif // CLIENT

	ComponentHandle<ProjectileLauncher> launcher_pattern;
	for (Entity entity : es.entities_with_components(launcher_pattern))
	{
		if (shoot)
		{
			Transform worldTransform = Transforms::getWorldTransform(entity);
			Entity projectile = es.create();
			projectile.assign<Transform>(worldTransform);
			projectile.assign<World>(Transforms::getWorldId(entity));
			projectile.assign<Projectile>()->velocity = worldTransform.getForward() * 1000.0;
			projectile.assign<Model>("res/models/bullet.obj", "res/textures/Red.png", "res/shaders/Textured", "");
			projectile.assign<TimeToLive>(1.0);
		}
	}

	ComponentHandle<Projectile> projectile_pattern;
	for (Entity entity : es.entities_with_components(projectile_pattern))
	{
		ComponentHandle<Projectile> projectile = entity.component<Projectile>();
		Transform transform = Transforms::getLocalTransform(entity);

		vector3D start_pos = transform.getPosition();
		vector3D end_pos = start_pos + (projectile->velocity * delta_time);

		//Update Projectile Position
		transform.setPosition(end_pos);
		Transforms::setLocalTransform(entity, transform);
	}
}
