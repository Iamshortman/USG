#ifndef COLLISIONSHAPE_HPP
#define COLLISIONSHAPE_HPP

#include "Common/Component/ComponentNode.hpp"
#include "Common/GLM_Include.hpp"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include "Common/Types.hpp"

enum CollisionShapeType
{
	Box,
	Capsule,
	ConvexMesh,
	ConcaveMesh,
};

class CollisionShape : public ComponentNode
{
public:
	CollisionShape(CollisionShapeType type, vector3D vec);
	CollisionShape(CollisionShapeType type, vector2D vec);
	CollisionShape(CollisionShapeType type, string string);

	~CollisionShape();

	btCollisionShape* getShape();

	virtual void addToEntity();
	virtual void removeFromEntity();

private:
	void setBox(vector3D half_length);
	void setCapsule(double radius, double height);
	void setConvexMesh(string file_path);
	void setConcaveMesh(string file_path);

	CollisionShapeType shape_type;
	btCollisionShape* shape = nullptr;

	int index;
};

#endif //COLLISIONSHAPE_HPP