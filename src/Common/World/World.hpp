#ifndef WORLD_HPP
#define WORLD_HPP

//Prototype Class
class World;

#include <vector>
#include <set>
#include <algorithm>

#include "Common/Entity/Entity.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

typedef uint32_t WorldId;

enum WORLDTYPE
{
	BASE,
	SOLAR,
	WARP,
};


class World
{
public:
	//TODO Figure out where better to put this
	vector3F ambientLight = vector3F(0.4f);

	const WorldId worldId;
	World(WorldId id);
	virtual ~World();
	void update(double deltaTime);

	void addEntityToWorld(Entity* entity);
	void removeEntityFromWorld(Entity* entity);
	std::set<Entity*>* getEntitiesInWorld();

	void addRigidBody(RigidBody* rigidBody);
	void removeRigidBody(RigidBody* entity);

	void addSubWorld(World* world);
	void removeSubWorld(World* world);
	std::set<World*>* getSubWorlds();

	vector3D getGravity();
	void setGravity(vector3D gravity);

	void setParent(Entity* entity);
	Entity* getParent();

	bool hasParentWorld();
	World* getParentWorld();

	Transform getWorldOffsetMatrix();

	virtual WORLDTYPE getWorldType() const;

private:
	PhysicsWorld* physicsWorld = nullptr;

	Entity* parent = nullptr;

	std::set<Entity*> entitiesInWorld;
	std::set<World*> subWorlds;
	World* parentWorld = nullptr;
};

#endif //WORLD_HPP