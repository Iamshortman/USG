#include "Common/World/WorldSolarSystem.hpp"

WorldSolarSystem::WorldSolarSystem(WorldId Id)
	:World(Id)
{

}

WorldSolarSystem::~WorldSolarSystem()
{
	for (Star* star : this->stars)
	{
		delete star;
	}

	for (Planet* planet : this->planets)
	{
		delete planet;
	}
}


void WorldSolarSystem::addStar(Star* star)
{
	this->stars.insert(star);
}

void WorldSolarSystem::removeStar(Star* star)
{
	this->stars.erase(star);
}

void WorldSolarSystem::addPlanet(Planet* planet)
{
	this->planets.insert(planet);
}

void WorldSolarSystem::removePlanet(Planet* planet)
{
	this->planets.erase(planet);
}

WORLDTYPE WorldSolarSystem::getWorldType() const
{
	return WORLDTYPE::SOLAR;
}