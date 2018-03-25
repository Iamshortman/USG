#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class GameObject;
class Component
{
public:
	Component() {};
	virtual void update(double delta_time) {};

	GameObject* getParent() { return this->parent; };

	virtual void enable() { this->enabled = true; };
	virtual void disable() { this->enabled = false; };
	virtual bool isEnabled() { return this->enabled; };

protected:
	GameObject * parent;
	bool enabled = false;

	friend class GameObject;
};

#endif //COMPONENT_HPP