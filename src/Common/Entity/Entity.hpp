#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>

#include "Common/GLM_Include.hpp"
#include "Common/Transform.hpp"

enum ENTITYTYPE
{
	CHARACTOR,
	PROP,
	GRIDSYSTEM,
};

//Prototype Classe
class World;

typedef uint32_t EntityId;

class Entity
{
public:

	const EntityId entityId;
	World* world = nullptr;
	bool alive = true;
	Transform transform;

	Entity(EntityId id);
	virtual ~Entity();

	bool isAlive() { return alive; };
	void kill();
	
	virtual void update(double deltaTime) = 0;
	virtual void addToWorld(World* world);
	virtual World* getWorld();
	virtual Transform getTransform();
	virtual void setTransform(Transform transform);
	Transform getRenderTransform();

	virtual ENTITYTYPE getEntityType() const = 0;
};

#endif //ENTITY_HPP