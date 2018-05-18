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

	for (GameObject* child : this->children)
	{
		child->update(delta_time);
	}
}

void GameObject::addChild(GameObject* game_object)
{
	this->children.insert(game_object);
	game_object->onParentChange(this);
}

void GameObject::removeChild(GameObject* game_object)
{
	game_object->onParentChange(nullptr);
	this->children.erase(game_object);
}

void GameObject::onParentChange(GameObject* new_parent)
{
	for (auto component : this->component_map)
	{
		component.second->disable();
	}

	this->parent = new_parent;

	for (auto component : this->component_map)
	{
		component.second->enable();
	}

	//Not sure if this is needed
	for (GameObject* child : this->children)
	{
		child->onParentChange(this);
	}
}

void GameObject::setLocalTransform(Transform transform)
{
	this->local_transform = transform;
}

Transform GameObject::getLocalTransform()
{
	if (this->hasComponent<RigidBody>())
	{
		this->local_transform = this->getComponent<RigidBody>()->getWorldTransform();
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

Transform GameObject::getRelitiveTransform(GameObject* parent)
{
	Transform relative_transform = this->getLocalTransform();

	GameObject* game_object = this->parent;

	while (game_object != nullptr)
	{
		if (game_object == parent)
		{
			//Found the parent, done searching
			break;
		}
		else if (game_object == nullptr)
		{
			//Didn't find the parent, not good
			Logger::getInstance()->logError("getRelitiveTransform parent not found\n");
		}

		relative_transform = relative_transform.transformBy(game_object->local_transform);
	}

	return relative_transform;
}
