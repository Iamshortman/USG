#include "Common/Entity/Entity.hpp"

#include "Common/Physics/RigidBody.hpp"

class EntityCharacter: public Entity
{
public:
	EntityCharacter(EntityId id);
	~EntityCharacter();

	RigidBody* getRigidBody();

	virtual void update(double deltaTime);
	virtual Transform getTransform();
	virtual void setTransform(Transform transform);
	virtual void addToWorld(World* world);

	virtual ENTITYTYPE getEntityType() const;

private:
	RigidBody* rigidBody = nullptr;
};