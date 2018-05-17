#ifndef COLLISIONSHAPE_HPP
#define COLLISIONSHAPE_HPP

#include "Common/Component.hpp"
#include "Common/GLM_Include.hpp"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

enum CollisionShapeType
{
	Box,
	Capsule,
};

class CollisionShape: public Component
{
public:
	CollisionShape();
	~CollisionShape();

	virtual void enable() override;
	virtual void disable() override;

	void setBox(vector3D half_length);
	void setCapsule(double radius, double height);

	btCollisionShape* getShape();

private:

	CollisionShapeType shape_type;
	btCollisionShape* shape = nullptr;
};

#endif //COLLISIONSHAPE_HPP