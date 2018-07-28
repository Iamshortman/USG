#include "CollisionShape.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/Node.hpp"
#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/SingleRigidBody.hpp"
#include "Common/Physics/MuiltiRigidBody.hpp"

#include "Common/Resource/Assimp_Include.hpp"
#include "Common/Logger/Logger.hpp"

CollisionShape::CollisionShape(I_Node* node)
	:ComponentNode(node)
{
	this->enabled = false;
	this->enable();
}

CollisionShape::~CollisionShape()
{
	this->disable();
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

	this->shape_type = ConvexMesh;
	this->shape = hull_shape;

	this->enable();
}

void CollisionShape::setConcaveMesh(string file_path)
{
	this->disable();

	if (this->shape != nullptr)
	{
		delete this->shape;
	}

	btTriangleMesh* triMesh = nullptr;

	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(file_path, aiProcess_Triangulate);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		Logger::getInstance()->logError("ASSIMP:: %s\n", import.GetErrorString());
		return;
	}

	if (scene->HasMeshes())
	{
		//Only Support 1 mesh for now
		aiMesh* mesh = scene->mMeshes[0];

		vector<vector3D> vertices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vector3D vertex;
			vertex.x = mesh->mVertices[i].x;
			vertex.y = mesh->mVertices[i].y;
			vertex.z = mesh->mVertices[i].z;
			vertices.push_back(vertex);
		}

		triMesh = new btTriangleMesh();
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			triMesh->addTriangle(toBtVec3(vertices[face.mIndices[0]]), toBtVec3(vertices[face.mIndices[1]]), toBtVec3(vertices[face.mIndices[2]]), true);
		}
	}

	import.FreeScene();

	this->shape_type = ConcaveMesh;
	this->shape = new btBvhTriangleMeshShape(triMesh, true);

	this->enable();
}

btCollisionShape* CollisionShape::getShape()
{
	return this->shape;
}

void CollisionShape::enable()
{
	if (this->shape == nullptr)
	{
		//Null shape don't bother
		return;
	}

	if (!this->enabled)
	{
		RigidBody* rigid_body = this->parent_node->getEntity()->getRigidBody();
		if (rigid_body != nullptr)
		{
			if (rigid_body->getType() == RigidBodyType::SINGLE)
			{
				SingleRigidBody* single_body = (SingleRigidBody*)rigid_body;
				single_body->setShape(this);
			}
			else if (rigid_body->getType() == RigidBodyType::MULTI)
			{
				MuiltiRigidBody* muilti_body = (MuiltiRigidBody*)rigid_body;
				muilti_body->addChildShape(this->parent_node);
			}

			ComponentNode::enable();
			return;
		}
	}

	Logger::getInstance()->logError("Shape could not attach\n");
}

void CollisionShape::disable()
{
	if (this->enabled && this->shape != nullptr)
	{
		RigidBody* rigid_body = this->parent_node->getEntity()->getRigidBody();
		if (rigid_body != nullptr)
		{
			if (rigid_body->getType() == RigidBodyType::SINGLE)
			{
				SingleRigidBody* single_body = (SingleRigidBody*)rigid_body;
				single_body->setShape(nullptr);
			}
			else if (rigid_body->getType() == RigidBodyType::MULTI)
			{
				MuiltiRigidBody* muilti_body = (MuiltiRigidBody*)rigid_body;
				muilti_body->removeChildShape(this->parent_node);
			}
		}
	}

	ComponentNode::disable();
}
