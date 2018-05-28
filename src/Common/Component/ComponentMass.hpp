#ifndef COMPONENT_MASS_HPP
#define COMPONENT_MASS_HPP

#include "Common/Component.hpp"
#include <set>

class ComponentMass : public Component
{
public:
	ComponentMass(double mass);
	virtual ~ComponentMass();

	virtual void enable() override;
	virtual void disable() override;

	double getTotalMass();

	void addChildMass(ComponentMass* child);
	void removeChildMass(ComponentMass* child);

private:
	double mass;
	std::set<ComponentMass*> children;
};

#endif // COMPONENT_MASS_HPP