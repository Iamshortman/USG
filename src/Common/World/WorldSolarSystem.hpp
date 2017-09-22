#ifndef WORLDSOLARSYSTEM_HPP
#define WORLDSOLARSYSTEM_HPP

#include "Common/World/World.hpp"

//Prototype
class Star {};
class Planet {};

class WorldSolarSystem : public World
{
public:
	WorldSolarSystem(WorldId Id);
	~WorldSolarSystem();

	virtual WORLDTYPE getWorldType() const;

	void addStar(Star* star);
	void removeStar(Star* star);

	void addPlanet(Planet* planet);
	void removePlanet(Planet* planet);

private:
	std::set<Star*> stars;
	std::set<Planet*> planets;
};

#endif //WORLDSOLARSYSTEM_HPP