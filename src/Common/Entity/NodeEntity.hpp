#ifndef NODE_ENTITY_HPP
#define NODE_ENTITY_HPP

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/Node.hpp"

#include <set>

class NodeEntity : public Entity
{
public:
	virtual ~NodeEntity();

	virtual void update(double delta_time);

	virtual RigidBody* addRigidBody();
	virtual void removeRigidBody();

	void addChild(Node* node);
	void removeChild(Node* node);

	inline std::set<Node*> getChildNodes() { return this->child_nodes; };

	virtual EntityType getType() { return EntityType::NODE_ENTITY; };

protected:

	std::set<Node*> child_nodes;

	NodeEntity(EntityId objectId); //protected for EntityManager use only

	friend class EntityManager;
};


#endif //NODE_ENTITY_HPP