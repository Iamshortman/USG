#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <set>
#include <map>

#include "Common/Transform.hpp"
#include "Common/Component.hpp"

typedef uint32_t GameObjectId;

class GameObject
{
public:
	virtual ~GameObject();

	virtual void update(double delta_Time);

	//GameObject* getParent();
	virtual void addChild(GameObject* game_object);
	virtual void removeChild(GameObject* game_object);

	virtual void onParentChange(GameObject* new_parent);

	template<typename T, typename... TArgs> void addComponent(TArgs&&... mArgs)
	{
		if (!this->hasComponent<T>())
		{
			Component* component = new T(std::forward<TArgs>(mArgs)...);
			component->parent = this;
			component->enable();
			this->component_map[typeid(T).hash_code()] = component;
		}
		else
		{
			printf("Error: Entity already has component %s\n", typeid(T).name());
		}
	};

	template<typename T, typename... TArgs> void addComponentNoEnable(TArgs&&... mArgs)
	{
		if (!this->hasComponent<T>())
		{
			Component* component = new T(std::forward<TArgs>(mArgs)...);
			component->parent = this;
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

	//Returns the first parent object with this component
	template<typename T> GameObject* findParentWith()
	{
		GameObject* game_object = this->parent;

		while (game_object != nullptr)
		{
			if (game_object->hasComponent<T>())
			{
				return game_object;
			}

			game_object = game_object->parent;
		}

		return nullptr;
	}

	//Returns the first parent object with this component
	//If the second component is found first return nullptr
	template<typename T, typename S> GameObject* findParentWithFirst_StopIfSecond()
	{
		GameObject* game_object = this->parent;

		while (game_object != nullptr)
		{
			if (game_object->hasComponent<S>())
			{
				//Found the second component
				return nullptr;
			}

			if (game_object->hasComponent<T>())
			{
				return game_object;
			}

			game_object = game_object->parent;
		}

		return nullptr;
	}

	virtual void setLocalTransform(Transform transform);
	virtual Transform getLocalTransform();
	Transform getGlobalTransform();
	Transform getRelitiveTransform(GameObject* parent);


	GameObject* parent = nullptr;
	std::set<GameObject*> children;
	std::map<size_t, Component*> component_map;

	const GameObjectId object_Id;
protected:
	Transform local_transform;

	GameObject(GameObjectId objectId); //protected for GameObjectManager use only
	friend class GameObjectManager;
};


#endif //GAME_OBJECT_HPP
