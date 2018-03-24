#ifndef WORLD_HPP
#define WORLD_HPP

//Prototype Class
class World;

#include <vector>
#include <set>
#include <algorithm>

#include "Common/Entity/Entity.hpp"
#include "Common/Physics/PhysicsWorld.hpp"
#include "Common/Entity/ComponentLight.hpp"

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
	virtual void update(double deltaTime);

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

	SingleRayTestResult singleRayTest(vector3D startPos, vector3D endPos);
	SingleRayTestResult singleRayTestNotMe(vector3D startPos, vector3D endPos, Entity* me);

	void setParent(Entity* entity);
	Entity* getParent();

	bool hasParentWorld();
	World* getParentWorld();

	Transform getWorldOffsetMatrix();

	virtual WORLDTYPE getWorldType() const;

	std::set<ComponentLight*> lightsInWorld;

protected:
	PhysicsWorld* physicsWorld = nullptr;

	Entity* parent = nullptr;

	std::set<Entity*> entitiesInWorld;
	std::set<World*> subWorlds;
	World* parentWorld = nullptr;
};

#endif //WORLD_HPP