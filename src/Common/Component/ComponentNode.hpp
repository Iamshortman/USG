#ifndef NODE_COMPONENT_HPP
#define NODE_COMPONENT_HPP

class I_Node;

class ComponentNode
{
public:
	ComponentNode(I_Node* node) : parent_node(node) {};

	virtual void update(double delta_time) {};

	virtual void enable() { this->enabled = true; };
	virtual void disable() { this->enabled = false; };
	virtual bool isEnabled() { return this->enabled; };

	I_Node* const parent_node;

protected:
	bool enabled = true;

	friend class I_Node;
};

#endif //NODE_COMPONENT_HPP