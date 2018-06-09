#include "GameObject.hpp"

#include "Common/Physics/RigidBody.hpp"

#include "Common/Logger/Logger.hpp"

GameObject::GameObject(GameObjectId objectId)
	: object_Id(objectId)
{
}

GameObject::~GameObject()
{

}

void GameObject::update(double delta_time)
{
	for (auto component : this->component_map)
	{
		component.second->update(delta_time);
	}

}

void GameObject::setLocalTransform(Transform transform)
{
	this->local_transform = transform;
}

Transform GameObject::getLocalTransform()
{
	return this->local_transform;
}

Transform GameObject::getGlobalTransform()
{
	Transform global_transform = this->getLocalTransform();

	return global_transform;
}
