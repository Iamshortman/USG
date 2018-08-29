#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include "Common/EntityX_Include.hpp"

#include "Common/Transform.hpp"
#include "Common/World/WorldSystem.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/World/NodeSystem.hpp"

namespace Transforms
{
	inline void setLocalTransform(Entity entity, Transform& transform)
	{
		if (entity.has_component<RigidBody>())
		{
			entity.component<RigidBody>().get()->setWorldTransform(transform);
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
		if (entity.has_component<RigidBody>())
		{
			return entity.component<RigidBody>().get()->getWorldTransform();
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

	inline Transform getParentRelativeTransform(Entity entity)
	{
		if (entity.has_component<Node>())
		{
			return entity.component<Node>()->local_transform.transformBy(getParentRelativeTransform(entity.component<Node>()->parent_entity));
		}

		return Transform(); //Root of the tree is at orgin
	}

	inline Transform getWorldTransform(Entity entity)
	{
		if (entity.has_component<Node>())
		{
			return entity.component<Node>()->local_transform.transformBy(getWorldTransform(entity.component<Node>()->parent_entity));
		}
		else
		{
			return getLocalTransform(entity);
		}
	}

	inline Transform getGlobalTransform(Entity entity)
	{
		Transform transform = getLocalTransform(entity);

		if (entity.has_component<World>() && entity.component<World>()->world_id != INVALID_WORLD)
		{
			Entity parent = WorldList::getInstance()->getWorldHost(entity.component<World>()->world_id);
			transform = transform.transformBy(getGlobalTransform(parent));
		}
		else if(entity.has_component<Node>())
		{
			transform = transform.transformBy(getGlobalTransform(entity.component<Node>()->parent_entity));
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
			return getWorldId(entity.component<Node>()->parent_entity);
		}

		return INVALID_WORLD;
	}

	inline Entity getRootParentEntity(Entity entity)
	{
		if (entity.has_component<Node>() && entity.component<Node>()->parent_entity.valid())
		{
			return getRootParentEntity(entity.component<Node>()->parent_entity);
		}

		return entity;
	}

};

#endif //TRANSFORMS_HPP