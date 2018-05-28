#include "ComponentMass.hpp"

#include "Common/GameObject.hpp"

ComponentMass::ComponentMass(double mass)
{
	this->mass = mass;
}

ComponentMass::~ComponentMass()
{
}

void ComponentMass::enable()
{
	if (!this->enabled)
	{
		GameObject* game_object = this->parent->findParentWith<ComponentMass>();

		if (game_object != nullptr)
		{
			ComponentMass* parent_mass = game_object->getComponent<ComponentMass>();

			if (parent_mass != nullptr)
			{
				parent_mass->addChildMass(this);
			}
		}

		Component::enable();
	}
}

void ComponentMass::disable()
{
	if (this->enabled)
	{
		ComponentMass* parent_mass = this->parent->findParentWith<ComponentMass>()->getComponent<ComponentMass>();

		if (parent_mass != nullptr)
		{
			parent_mass->removeChildMass(this);
		}

		Component::disable();
	}
}

double ComponentMass::getTotalMass()
{
	double total = this->mass;
	for (ComponentMass* child : this->children)
	{
		total += child->getTotalMass();
	}
	return total;
}

void ComponentMass::addChildMass(ComponentMass* child)
{
	this->children.emplace(child);
}

void ComponentMass::removeChildMass(ComponentMass* child)
{
	this->children.erase(child);
}
