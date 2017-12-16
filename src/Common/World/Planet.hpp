#ifndef PLANET_HPP
#define PLANET_HPP

#include "Common/GLM_Include.hpp"
#include "Common/World/World.hpp"

class Planet
{
public:
	Planet(World* world, vector3D postion, double mass);
	~Planet();

	virtual void update(double deltaTime);

private:
	vector3D postion;
	double mass;
	
	World* parent;

	const double UNIVERSAL_GRAVITY_CONST = 0.00000000006673;
};

#endif //PLANET_HPP