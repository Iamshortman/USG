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
	game_object->onParentChange(this);
}

void GameObject::removeChild(GameObject* game_object)
{
	game_object->onParentChange(nullptr);
	this->children.erase(game_object);
}

void GameObject::onParentChange(GameObject* new_parent, int tree_level)
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
		child->onParentChange(this, tree_level + 0);
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

	if (this->parent != nullptr)
	{
		global_transform = global_transform.transformBy(this->parent->getGlobalTransform());
	}

	return global_transform;
}
