#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class Node;
class EntityNode;

enum ComponentType
{
	DEFAULT,
	MODEL,
	COLLISIONSHAPE,
};

class Component 
{
public:
	virtual ~Component() {};

	virtual void update(double deltaTime) {};
	virtual void enable() { this->enabled = true; };
	virtual void disable() { this->enabled = false; };

	virtual void setParentNode(Node* node) { this->parent = node; };
	Node* getParentNode() const { return this->parent; };

	virtual void addtoEntity(EntityNode* entity) { this->parent_entity = entity; };

	virtual ComponentType getComponentType() const = 0;

protected:
	Node* parent = nullptr;
	EntityNode* parent_entity = nullptr;
	bool enabled = false;
};

#endif //COMPONENT_HPP