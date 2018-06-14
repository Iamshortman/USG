#include "ComponentMass.hpp"

#include "Common/Entity/Entity.hpp"

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
		Component::enable();
	}
}

void ComponentMass::disable()
{
	if (this->enabled)
	{
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
