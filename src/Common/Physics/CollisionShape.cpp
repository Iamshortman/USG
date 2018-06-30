#include "CollisionShape.hpp"

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/SingleRigidBody.hpp"
#include "Common/Physics/MuiltiRigidBody.hpp"

#include "Common/Resource/Assimp_Include.hpp"
#include "Common/Logger/Logger.hpp"

CollisionShape::CollisionShape(I_Node* node)
	:ComponentNode(node)
{
}

CollisionShape::~CollisionShape()
{
}

void CollisionShape::setBox(vector3D half_length)
{
	this->disable();

	if (this->shape != nullptr)
	{
		delete this->shape;
	}

	this->shape_type = Box;
	this->shape = new btBoxShape(toBtVec3(half_length));

	this->enable();
}

void CollisionShape::setCapsule(double radius, double height)
{
	this->disable();

	if (this->shape != nullptr)
	{
		delete this->shape;
	}

	this->shape_type = Capsule;
	this->shape = new btCapsuleShape(radius, height);

	this->enable();
}

void CollisionShape::setConvexMesh(string file_path)
{
	this->disable();

	if (this->shape != nullptr)
	{
		delete this->shape;
	}

	this->shape_type = ConvexMesh;

	btConvexHullShape* hull_shape = new btConvexHullShape();

	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		Logger::getInstance()->logError("ASSIMP:: %s\n", import.GetErrorString());
		return;
	}

	if (scene->HasMeshes())
	{
		//Only Support 1 mesh for now
		aiMesh* mesh = scene->mMeshes[0];

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			hull_shape->addPoint(btVector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z), false);
		}

		hull_shape->recalcLocalAabb();
	}

	this->shape = hull_shape;

	this->enable();
}

btCollisionShape* CollisionShape::getShape()
{
	return this->shape;
}

/*void CollisionShape::enable()
{
	if (!this->enabled && this->shape != nullptr)
	{
		if (this->parent_entity != nullptr)
		{
			RigidBody* rigid_body = this->parent_entity->getRigidBody();
			if (rigid_body != nullptr)
			{
				if (rigid_body->getType() == RigidBodyType::SINGLE)
				{
					SingleRigidBody* single_body = (SingleRigidBody*)rigid_body;
					single_body->setShape(this);
				}
			}
		}
		else if (this->parent_node != nullptr)
		{
			if (this->parent_node->getParentEntity() != nullptr)
			{
				this->shape->setUserPointer(this->parent_node);

				RigidBody* rigid_body = this->parent_node->getParentEntity()->getRigidBody();
				if (rigid_body != nullptr)
				{
					if (rigid_body->getType() == RigidBodyType::MULTI)
					{
						MuiltiRigidBody* muilti_body = (MuiltiRigidBody*)rigid_body;
						muilti_body->addChildShape(this->parent_node);
					}
				}
			}
		}
	}
}*/

/*void CollisionShape::disable()
{
	if (this->enabled && this->shape != nullptr)
	{
		if (this->parent_entity != nullptr)
		{
			RigidBody* rigid_body = this->parent_entity->getRigidBody();
			if (rigid_body != nullptr)
			{
				if (rigid_body->getType() == RigidBodyType::SINGLE)
				{
					SingleRigidBody* single_body = (SingleRigidBody*)rigid_body;
					single_body->setShape(nullptr);
				}
			}
		}
		else if (this->parent_node != nullptr)
		{
			if (this->parent_node->getParentEntity() != nullptr)
			{
				RigidBody* rigid_body = this->parent_node->getParentEntity()->getRigidBody();
				if (rigid_body != nullptr)
				{
					if (rigid_body->getType() == RigidBodyType::MULTI)
					{
						MuiltiRigidBody* muilti_body = (MuiltiRigidBody*)rigid_body;
						muilti_body->removeChildShape(this->parent_node);
					}
				}
			}
		}
	}

	Component::disable();
}*/
