#include "Common/Component/Component.hpp"
#include "Common/Entity/Entity.hpp"

Component::Component()
{

}

void Component::update(double deltaTime)
{

};

Entity* Component::getParent()
{
	return this->parent;
};

void Component::setParent(Entity* entity)
{
	this->parent = entity;
};

Component::~Component()
{

}