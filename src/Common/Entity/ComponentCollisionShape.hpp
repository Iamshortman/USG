#include "Common/Entity/Component.hpp"
#include "Common/Physics/CollisionShape.hpp"
#include "Common/Transform.hpp"
#include "Common/Physics/RigidBody.hpp"


class ComponentCollisionShape : public Component
{
public:
	ComponentCollisionShape(CollisionShape* shape, double mass);
	virtual ~ComponentCollisionShape();

	virtual void update(double deltaTime);

	virtual void enable() override;
	virtual void disable() override;

	virtual void addtoEntity(EntityNode* entity);

	virtual ComponentType getComponentType() const override;

protected:
	CollisionShape* shape;
	double mass;

	childId childShapeId;
	Transform current_transform;
};