#ifndef COMPONET_HPP
#define COMPONET_HPP

//Prototype class
class Entity;
class World;

class Component
{
private:
	Entity* parent;

public:
	Component();
	virtual ~Component();

	virtual void update(double deltaTime);

	virtual void addToWorld(World* world);
	virtual void removeFromWorld(World* world);

	Entity* getParent();
	void setParent(Entity* entity);
};

#endif //COMPONET_HPP