#ifndef COMPONET_HPP
#define COMPONET_HPP

//Prototype class
class Entity;

class Component
{
private:
	Entity* parent;

public:
	Component();
	virtual ~Component();

	virtual void update(double deltaTime);

	Entity* getParent();
	void setParent(Entity* entity);
};

#endif //COMPONET_HPP