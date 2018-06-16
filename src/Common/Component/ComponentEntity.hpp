#ifndef ENTITY_COMPONENT_HPP
#define ENTITY_COMPONENT_HPP

#include "Common/Component/Component.hpp"

class Entity;
class ComponentEntity : public virtual Component
{
public:
	Entity* getParentEntity() { return this->parent_entity; };

protected:
	Entity* parent_entity;

	friend class Entity;
};

#endif //ENTITY_COMPONENT_HPP