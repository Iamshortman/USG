#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Common/Entity/I_Node.hpp"

#include "Common/Transform.hpp"
#include "Common/Component/ComponentEntity.hpp"
#include "Common/Physics/RigidBody.hpp"

#include <set>
#include <map>

enum EntityType
{
	ENTITY,
	NODE_ENTITY,
};

//Prototype Classe
class World;

typedef uint32_t EntityId;

class Entity : public I_Node
{
public:
	virtual ~Entity();

	inline bool isAlive() { return this->alive; };
	inline void kill() { this->alive = false; };

	virtual void update(double delta_time);

	virtual Node* getNode() { return nullptr; };
	virtual Entity* getEntity() { return this; };

	virtual void setLocalTransform(Transform transform);
	virtual Transform getLocalTransform();
	virtual Transform getRelativeTransform();
	virtual Transform getWorldTransform();
	virtual Transform getGlobalTransform();

	template<typename T, typename... TArgs> T* addComponent(TArgs&&... mArgs)
	{
		if (!this->hasComponent<T>())
		{
			ComponentEntity* component = new T(this, std::forward<TArgs>(mArgs)...);
			//component->enable();
			this->component_map[typeid(T).hash_code()] = component;
			return (T*)component;
		}
		else
		{
			printf("Error: Entity already has component %s\n", typeid(T).name());
			return nullptr;
		}
	};

	template<typename T> bool hasComponent()
	{
		return this->component_map.find(typeid(T).hash_code()) != this->component_map.end();
	};

	template<typename T> T* getComponent()
	{
		if (this->hasComponent<T>())
		{
			return (T*)this->component_map[typeid(T).hash_code()];
		}
		return nullptr;
	};

	template<typename T> void removeComponent()
	{
		if (this->hasComponent<T>())
		{
			size_t type = typeid(T).hash_code();
			delete this->component_map[type];
			this->component_map.erase(type);
		}
	};

	virtual void addToWorld(World* world);
	inline World* getWorld() { return this->world; };

	virtual void addRigidBody();
	virtual void removeRigidBody();
	inline RigidBody* getRigidBody() { return this->rigidBody; };

	virtual EntityType getType() { return EntityType::ENTITY; };

	const EntityId entityId;
protected:
	std::map<size_t, ComponentEntity*> component_map;

	Transform local_transform;

	World* world = nullptr;
	bool alive = true;

	RigidBody* rigidBody = nullptr;

	Entity(EntityId objectId); //protected for EntityManager use only
	friend class EntityManager;
};


#endif //ENTITY_HPP
