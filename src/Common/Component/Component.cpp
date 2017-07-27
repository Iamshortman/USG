#include "Common/Component/Component.hpp"
#include "Common/Entity/Entity.hpp"
#include "Common/World/World.hpp"

Component::Component()
{

}

void Component::update(double deltaTime){};
void Component::addToWorld(World* world){};
void Component::removeFromWorld(World* world){};

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