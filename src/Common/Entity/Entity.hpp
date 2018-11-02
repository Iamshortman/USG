#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Common/Transform.hpp"
#include "Common/Component/ComponentEntity.hpp"

#include <set>
#include <map>

enum EntityType
{
	ENTITY,
};

//Prototype Classe
class World;

typedef uint32_t EntityId;

class Entity
{
public:
	virtual ~Entity();

	inline bool isAlive() { return this->alive; };
	inline void kill() { this->alive = false; };

	virtual void update(double delta_time);

	virtual void setLocalTransform(Transform transform);
	virtual Transform getLocalTransform();
	virtual Transform getRelativeTransform();
	virtual Transform getWorldTransform();
	virtual Transform getGlobalTransform();

	virtual void addToWorld(World* world);
	World* getWorld();

	const EntityId entityId;
protected:
	Transform local_transform;

	World* world = nullptr;
	bool alive = true;

	Entity(EntityId objectId); //protected for EntityManager use only
	friend class EntityManager;
};


#endif //ENTITY_HPP
