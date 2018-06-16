#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <set>
#include <map>

#include "Common/Transform.hpp"
#include "Common/Component/ComponentEntity.hpp"
#include "Common/Physics/RigidBody.hpp"

//Prototype Classe
class World;

typedef uint32_t EntityId;

class Entity
{
public:
	virtual ~Entity();

	inline bool isAlive() { return this->alive; };
	inline void kill() { this->alive = false; };

	virtual void update(double delta_Time);

	template<typename T, typename... TArgs> T* addComponent(TArgs&&... mArgs)
	{
		if (!this->hasComponent<T>())
		{
			ComponentEntity* component = new T(std::forward<TArgs>(mArgs)...);
			component->parent_entity = this;
			component->enable();
			this->component_map[typeid(T).hash_code()] = component;
			return (T*)component;
		}
		else
		{
			printf("Error: Entity already has component %s\n", typeid(T).name());
			return nullptr;
		}
	};

	template<typename T, typename... TArgs> void addComponentNoEnable(TArgs&&... mArgs)
	{
		if (!this->hasComponent<T>())
		{
			ComponentEntity* component = new T(std::forward<TArgs>(mArgs)...);
			component->parent_entity = this;
			this->component_map[typeid(T).hash_code()] = component;
		}
		else
		{
			printf("Error: Entity already has component %s\n", typeid(T).name());
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

	virtual void setLocalTransform(Transform transform);
	virtual Transform getLocalTransform();
	Transform getGlobalTransform();

	virtual void addToWorld(World* world);
	inline World* getWorld() { return this->world; };

	virtual void addRigidBody();
	virtual void removeRigidBody();
	inline RigidBody* getRigidBody() { return this->rigidBody; };

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
