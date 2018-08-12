#ifndef NODE_COMPONENT_HPP
#define NODE_COMPONENT_HPP

class I_Node;

class ComponentNode
{
public:
	ComponentNode() {};
	virtual void update(double delta_time) {};

	virtual void addToEntity() {};
	virtual void removeFromEntity() {};

	I_Node* parent_node = nullptr;

protected:
	friend class I_Node;
};

#endif //NODE_COMPONENT_HPP