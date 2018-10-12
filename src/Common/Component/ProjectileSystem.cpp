#include "Projectile.hpp"

#include "Common/Transforms.hpp"
#include "Common/Rendering/Model.hpp"
#include "Common/Component/TimeToLive.hpp"

#include "Common/Physics/PhysicsSystem.hpp"

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
			vector3D velocity = vector3D(0.0);

			Entity parent = Transforms::getRootParentEntity(entity);
			if (parent.has_component<RigidBody>())
			{
				velocity = parent.component<RigidBody>()->getLinearVelocity();
			}

			Transform worldTransform = Transforms::getWorldTransform(entity);
			WorldId worldId = Transforms::getWorldId(entity);

			Entity projectile = es.create();
			projectile.assign<Transform>(worldTransform);
			projectile.assign<World>(worldId);
			projectile.assign<Projectile>()->velocity = velocity + (worldTransform.getForward() * 1000.0);
			projectile.assign<Model>("res/models/bullet.obj", "res/textures/Red.png", "res/shaders/Textured", "res/shaders/Shadow");
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

		PhysicsWorld* physics_world = PhysicsWorldList::getInstance()->getPhysicsWorld(entity.component<World>()->world_id);
		if (physics_world != nullptr)
		{
			SingleRaycastResult result = physics_world->SingleRaycast(start_pos, end_pos, es);

			if (result.has_hit)
			{
				//entity.destroy();
				EntityDestroyer::destroyEntity(entity);
			}
		}
	}
}
