#ifndef NODE_HPP
#define NODE_HPP

#include "Common/Entity/I_Node.hpp"
#include "Common/Component/ComponentNode.hpp"
#include "Common/Entity/Entity.hpp"
#include "Common/Transform.hpp"
#include <set>
#include <map>

class NodeEntity;

class Node : public I_Node
{
public:
	Node();
	~Node();

	void update(double delta_time);

	virtual Node* getNode() { return this; };
	virtual Entity* getEntity() { return (Entity*)this->parent_entity; };

	virtual void setLocalTransform(Transform transform);
	virtual Transform getLocalTransform();
	virtual Transform getRelativeTransform();
	virtual Transform getWorldTransform();
	virtual Transform getGlobalTransform();

	void addToEntity(NodeEntity* entity);

	void addChild(Node* node);
	void removeChild(Node* node);

	inline std::set<Node*> getChildNodes() { return this->child_nodes; };

	inline Node* getParent() { return this->parent; };

protected:
	NodeEntity* parent_entity = nullptr;
	Node* parent = nullptr;

	Transform localTransform;

	std::set<Node*> child_nodes;

	friend NodeEntity;
};

#endif //NODE_HPP