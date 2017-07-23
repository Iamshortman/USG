#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <hash_map>
#include <string>
#include <vector>

#include "Common/Component/Component.hpp"
#include "Common/Component/Transform.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Physics/RigidBody.hpp"

//Prototype Classes
class World;

typedef uint32_t EntityId;

class Entity
{
public:

	const EntityId entityId;
	Entity(EntityId id);
	virtual ~Entity();

	bool isAlive() { return alive; };
	void kill();

	virtual void update(double deltaTime);

	void addToWorld(World* world);
	World* getWorld();

	//Transform Functions
	vector3D getScale();
	void setScale(vector3D scale);

	Transform getTransform();
	void setTransform(Transform transform);

	//Component functions 
	bool hasComponent(std::string componentName);
	Component* getComponent(std::string componentName);
	void addComponent(std::string componentName, Component* component);
	void removeComponent(std::string componentName);

	Transform getRenderTransform();

	bool hasSubWorld();
	World* getSubWorld();
	void setSubWorld(World* world);
	void removeSubWorld();

	void createRigidBody(double mass = 1.0, CollisionShape* shape = nullptr);
	void removeRigidBody();
	bool hasRigidBody();
	RigidBody* getRigidBody();

private:
	World* world = nullptr;
	std::hash_map<std::string, Component*> components;
	bool alive = true;

	Transform transform;
	RigidBody* rigidBody = nullptr;
	World* subWorld = nullptr;

};

#endif //ENTITY_HPP