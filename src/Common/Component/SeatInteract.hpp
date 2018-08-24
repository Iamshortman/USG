#ifndef INTERACT_HPP
#define INTERACT_HPP

#include "Common/Component/ComponentNode.hpp"
#include "Common/Entity/Entity.hpp"

class SeatInteract : public ComponentNode
{
public:
	SeatInteract();

	void EntityInteract(Entity* entity);

	void setSeat(I_Node* seat);

private:
	I_Node* node_seat = nullptr;
};

#endif //INTERACT_HPP