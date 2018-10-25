#include "Projectile.hpp"

#include "Common/Transforms.hpp"
#include "Common/Rendering/Model.hpp"
#include "Common/Rendering/InstancedModel.hpp"

#include "Common/Component/TimeToLive.hpp"

#include "Common/Physics/PhysicsSystem.hpp"

#ifdef CLIENT
#include "Client/Input/InputManager.hpp"
#endif // CLIENT

void LaunchProjectile(EntityManager& es, WorldId world, Transform transform, vector3D velocity)
{
	Entity projectile = es.create();
	projectile.assign<Transform>(transform);
	projectile.assign<World>(world);
	projectile.assign<Projectile>()->velocity = velocity + (transform.getForward() * 300.0);
	//projectile.assign<Model>("res/models/bullet.obj", "res/textures/Red.png", "res/shaders/Textured", "res/shaders/Shadow");
	projectile.assign<InstancedModel>("");
	projectile.assign<TimeToLive>(10.0);
};


void ProjectileSystem::update(EntityManager& es, EventManager& events, TimeDelta delta_time)
{
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

		/*PhysicsWorld* physics_world = PhysicsWorldList::getInstance()->getPhysicsWorld(entity.component<World>()->world_id);
		if (physics_world != nullptr)
		{
			SingleRaycastResult result = physics_world->SingleRaycast(start_pos, end_pos, es);

			if (result.has_hit)
			{
				//entity.destroy();
				EntityDestroyer::destroyEntity(entity);
			}
		}*/
	}

	bool shoot = false;

	double time_shoot = 1.0 / 10;
	static double time_left = time_shoot;

	time_left -= delta_time;

#ifdef CLIENT
	if (InputManager::getInstance()->getButtonDown("Flight_Shoot") && time_left <= 0.0)
	{
		shoot = true;
		time_left = time_shoot;
	}
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
			worldTransform.position += worldTransform.getForward();//Move forward a little bit;

			for (int i = -2; i < 2; i++)
			{
				worldTransform.position.y += 1.0;
				LaunchProjectile(es, worldId, worldTransform, velocity);
			}
		}
	}

}
