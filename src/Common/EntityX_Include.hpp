#ifndef ENTITYX_INCLUDE_HPP
#define ENTITYX_INCLUDE_HPP

#include "entityx/entityx.h"

using namespace entityx;


#include "Common/Transform.hpp"
#include "Common/Physics/RigidBody.hpp"

inline Transform getEntityTransform(Entity entity)
{
	if (entity.has_component<RigidBody>())
	{
		ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
		return rigidBody.get()->getWorldTransform();
	}

	if (entity.has_component<Transform>())
	{
		return *entity.component<Transform>().get();
	}

	return Transform();
}

#endif //ENTITYX_INCLUDE_HPP
