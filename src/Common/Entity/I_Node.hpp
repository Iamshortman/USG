#ifndef I_NODE_HPP
#define I_NODE_HPP

#include "Common/Component/ComponentNode.hpp"
#include "Common/Transform.hpp"
#include <map>

class Node;
class Entity;

class I_Node
{
public:

	~I_Node()
	{
		for (auto componet : this->node_component_map)
		{
			delete componet.second;
		}
	}

	void update(double delta_time)
	{
		for (auto component : this->node_component_map)
		{
			component.second->update(delta_time);
		}
	};

	virtual Node* getNode() = 0;
	virtual Entity* getEntity() = 0;

	virtual void setLocalTransform(Transform transform) = 0;
	virtual Transform getLocalTransform() = 0; //Local Space
	virtual Transform getRelativeTransform() = 0; //Entity Space
	virtual Transform getWorldTransform() = 0; //World Space
	virtual Transform getGlobalTransform() = 0; //Render Space

	template<typename T, typename... TArgs> T* addNodeComponent(TArgs&&... mArgs)
	{
		if (!this->hasNodeComponent<T>())
		{
			ComponentNode* component = new T(std::forward<TArgs>(mArgs)...);
			component->parent_node = this;
			this->node_component_map[typeid(T).hash_code()] = component;
			return (T*)component;
		}
		else
		{
			printf("Error: Node already has component %s\n", typeid(T).name());
			return nullptr;
		}
	};

	template<typename T> bool hasNodeComponent()
	{
		return this->node_component_map.find(typeid(T).hash_code()) != this->node_component_map.end();
	};

	template<typename T> T* getNodeComponent()
	{
		if (this->hasNodeComponent<T>())
		{
			return (T*)this->node_component_map[typeid(T).hash_code()];
		}
		return nullptr;
	};

	template<typename T> void removeNodeComponent()
	{
		if (this->hasNodeComponent<T>())
		{
			size_t type = typeid(T).hash_code();
			delete this->node_component_map[type];
			this->component_map.erase(type);
		}
	};

protected:
	std::map<size_t, ComponentNode*> node_component_map;
};

#endif //I_NODE_HPP