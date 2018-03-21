#ifndef NODE_HPP
#define NODE_HPP

#include "Common/Entity/Component.hpp"
#include "Common/Transform.hpp"
#include <set>
#include <map>

class EntityNode;

class Node
{
public:
	Node();
	~Node();

	void update(double deltaTime);

	Node* getParent();

	void addToEntity(EntityNode* entity);

	void addChild(Node* node);
	void removeChild(Node* node);

	void setLocalTransform(Transform trans);
	Transform getLocalTransform();

	Transform getTransform();

	void addComponent(Component *component);
	bool hasComponent(ComponentType type);
	Component* getComponent(ComponentType type);
	void removeComponent(ComponentType type);

	//void interactRay(Entity* entity, vector3D localStartPos, vector3D localHitPos, vector3D localHitNormal);

	EntityNode* getParentEntity();

protected:
	EntityNode* parent_entity = nullptr;

	Transform localTransform;

	Node* parent = nullptr;
	std::set<Node*> child_nodes;
	
	std::map<ComponentType, Component*> componentMap;
};

#endif //NODE_HPP