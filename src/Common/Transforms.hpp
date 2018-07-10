#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include "Common/EntityX_Include.hpp"

#include "Common/Transform.hpp"
#include "Common/WorldSystem.hpp"
#include "Common/Physics/SingleRigidBody.hpp"
#include "Common/NodeSystem.hpp"

namespace Transforms
{
	inline void setLocalTransform(Entity entity, Transform& transform)
	{
		if (entity.has_component<SingleRigidBody>())
		{
			entity.component<SingleRigidBody>().get()->setWorldTransform(transform);
		}

		if (entity.has_component<Transform>())
		{
			entity.component<Transform>()->setTransform(transform);
		}

		if (entity.has_component<Node>())
		{
			entity.component<Node>()->local_transform = transform;
		}
	}

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

		if (entity.has_component<Node>())
		{
			return entity.component<Node>()->local_transform;
		}

		return Transform();
	}

	inline Transform getGlobalTransform(Entity entity)
	{
		Transform transform = Transforms::getLocalTransform(entity);

		if (entity.has_component<World>())
		{
			Entity parent = WorldList::getInstance()->getWorldHost(entity.component<World>()->world_id);
			transform = transform.transformBy(Transforms::getGlobalTransform(parent));
		}
		else if(entity.has_component<Node>())
		{
			transform = transform.transformBy(Transforms::getGlobalTransform(entity.component<Node>()->parent_entity));
		}

		return transform;
	}

	inline WorldId getWorldId(Entity entity)
	{
		if (entity.has_component<World>())
		{
			return entity.component<World>()->world_id;
		}

		if (entity.has_component<Node>())
		{
			return Transforms::getWorldId(entity.component<Node>()->parent_entity);
		}

		return 0;//TODO invalid world id
	}

};

#endif //TRANSFORMS_HPP