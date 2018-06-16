#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class Component
{
public:
	Component() {};
	virtual void update(double delta_time) {};

	virtual void enable() { this->enabled = true; };
	virtual void disable() { this->enabled = false; };
	virtual bool isEnabled() { return this->enabled; };

protected:
	bool enabled = false;
};

#endif //COMPONENT_HPP