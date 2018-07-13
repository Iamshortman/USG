#include "CollisionShape.hpp"

#include "Common/Physics/Bullet_Include.hpp"
#include "Common/Physics/RigidBody.hpp"

#include "Common/Resource/Assimp_Include.hpp"
#include "Common/Logger/Logger.hpp"

CollisionShape CollisionShapes::createBox(vector3D half_length)
{
	return CollisionShape(new btBoxShape(toBtVec3(half_length)), CollisionShapeType::Box);
}

CollisionShape CollisionShapes::createCapsule(double radius, double height)
{
	return CollisionShape(new btCapsuleShape(radius, height), CollisionShapeType::Capsule);
}

CollisionShape CollisionShapes::createConvexMesh(string file_path)
{
	btConvexHullShape* hull_shape = new btConvexHullShape();

	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		Logger::getInstance()->logError("ASSIMP:: %s\n", import.GetErrorString());
		return CollisionShape(nullptr, CollisionShapeType::ConvexMesh);;
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

	return CollisionShape(hull_shape, CollisionShapeType::ConvexMesh);
}
