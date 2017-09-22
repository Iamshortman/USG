#include "Common/Entity/Entity.hpp"

#include "Common/Physics/RigidBody.hpp"

class EntityGridSystem : public Entity
{
public:
	EntityGridSystem(EntityId id);
	~EntityGridSystem();
	
	bool hasSubWorld();
	World* getSubWorld();
	void setSubWorld(World* world);
	void removeSubWorld();
	RigidBody* getRigidBody();

	virtual void update(double deltaTime);
	virtual Transform getTransform();
	virtual void setTransform(Transform transform);
	virtual void addToWorld(World* world);

	virtual ENTITYTYPE getEntityType() const;

private:
	RigidBody* rigidBody = nullptr;
	World* subWorld = nullptr;
};