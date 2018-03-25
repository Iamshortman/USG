#include "GameObject.hpp"

#include "Common/Physics/RigidBody.hpp"

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

	for (GameObject* child : this->children)
	{
		child->update(delta_time);
	}
}

void GameObject::addChild(GameObject* game_object)
{
	this->children.insert(game_object);
	game_object->parent = this;
}

void GameObject::removeChild(GameObject* game_object)
{
	this->children.erase(game_object);
	game_object->parent = nullptr;
}

void GameObject::setLocalTransform(Transform transform)
{
	this->local_transform = transform;

	RigidBody* rigidBody = this->getComponent<RigidBody>();
	if (rigidBody != nullptr)
	{
		rigidBody->setWorldTransform(this->local_transform);
	}
}

Transform GameObject::getLocalTransform()
{
	RigidBody* rigidBody = this->getComponent<RigidBody>();
	if (rigidBody != nullptr)
	{
		this->local_transform = rigidBody->getWorldTransform();
	}

	return this->local_transform;
}

Transform GameObject::getGlobalTransform()
{
	Transform global_transform = this->getLocalTransform();

	if (this->parent != nullptr)
	{
		global_transform = global_transform.transformBy(this->parent->getGlobalTransform());
	}

	return global_transform;
}
