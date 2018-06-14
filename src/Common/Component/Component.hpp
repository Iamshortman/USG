#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class Entity;
class Component
{
public:
	Component() {};
	virtual void update(double delta_time) {};

	Entity* getParent() { return this->parent; };

	virtual void enable() { this->enabled = true; };
	virtual void disable() { this->enabled = false; };
	virtual bool isEnabled() { return this->enabled; };

protected:
	Entity* parent;
	bool enabled = false;

	friend class Entity;
};

#endif //COMPONENT_HPP