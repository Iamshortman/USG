#ifndef MASS_HPP
#define MASS_HPP

#include "Common/Component/ComponentNode.hpp"

class Mass : public ComponentNode
{
public:
	Mass(double mass)
		:mass(mass) {};

	double getMass() { return this->mass; };

private:
	double mass;
};


#endif //MASS_HPP