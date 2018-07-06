#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include "Common/EntityX_Include.hpp"

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

		if (entity.has_component<WorldTransform>())
		{
			return entity.component<WorldTransform>()->transform;
		}

		return Transform();
	}

};

#endif //TRANSFORMS_HPP