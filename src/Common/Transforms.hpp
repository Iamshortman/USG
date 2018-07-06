#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include "Common/EntityX_Include.hpp"

#include "Common/Transform.hpp"
#include "Common/WorldSystem.hpp"
#include "Common/Physics/SingleRigidBody.hpp"

namespace Transforms
{
	inline Transform getLocalTransform(Entity entity)
	{
		if (entity.has_component<SingleRigidBody>())
		{
			return entity.component<SingleRigidBody>().get()->getWorldTransform();
		}

		if (entity.has_component<Transform>())
		{
			return *entity.component<Transform>().get();
		}

		return Transform();
	}

	inline Transform getGlobalTransform(Entity entity)
	{
		Transform transform = Transforms::getLocalTransform(entity);

		if (entity.has_component<World>())
		{
			Entity parent = WorldList::getInstance()->getWorldHost(entity.component<World>()->world_id);
			transform.transformBy(Transforms::getGlobalTransform(parent));
		}

		return transform;
	}

};

#endif //TRANSFORMS_HPP