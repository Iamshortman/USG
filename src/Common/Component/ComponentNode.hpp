#ifndef NODE_COMPONENT_HPP
#define NODE_COMPONENT_HPP

#include "Common/Component/Component.hpp"

class Node;
class ComponentNode : public virtual Component
{
public:
	inline Node* getParentNode() { return this->parent_node; };

protected:
	Node* parent_node;

	friend class Node;
};

#endif //NODE_COMPONENT_HPP