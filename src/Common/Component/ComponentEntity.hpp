#ifndef ENTITY_COMPONENT_HPP
#define ENTITY_COMPONENT_HPP

class Entity;

class ComponentEntity
{
public:
	ComponentEntity(Entity* entity) : parent_entity(entity) {};

	virtual void update(double delta_time) {};

	virtual void enable() { this->enabled = true; };
	virtual void disable() { this->enabled = false; };
	virtual bool isEnabled() { return this->enabled; };

	Entity* const parent_entity;

protected:
	bool enabled = true;

	friend class Entity;
};

#endif //ENTITY_COMPONENT_HPP