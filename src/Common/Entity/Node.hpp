#ifndef NODE_HPP
#define NODE_HPP

#include "Common/Component/ComponentNode.hpp"
#include "Common/Transform.hpp"
#include <set>
#include <map>

class NodeEntity;

class Node
{
public:
	Node();
	~Node();

	void update(double deltaTime);

	template<typename T, typename... TArgs> T* addComponent(TArgs&&... mArgs)
	{
		if (!this->hasComponent<T>())
		{
			ComponentNode* component = new T(std::forward<TArgs>(mArgs)...);
			component->parent_node = this;
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

	void addToEntity(NodeEntity* entity);

	void addChild(Node* node);
	void removeChild(Node* node);

	inline std::set<Node*> getChildNodes() { return this->child_nodes; };

	void setLocalTransform(Transform trans);
	Transform getLocalTransform();
	Transform getRelativeTransform();
	Transform getGlobalTransform();

	inline Node* getParent() { return this->parent; };
	inline NodeEntity* getParentEntity() { return this->parent_entity; };

protected:
	std::map<size_t, ComponentNode*> component_map;

	NodeEntity* parent_entity = nullptr;
	Node* parent = nullptr;

	Transform localTransform;

	std::set<Node*> child_nodes;

	friend NodeEntity;
};

#endif //NODE_HPP