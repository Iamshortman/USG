#ifndef CONCAVEMESHSHAPE_HPP
#define CONCAVEMESHSHAPE_HPP

#include "Common/Physics/CollisionShapes/CollisionShape.hpp"
#include "Common/Physics/Bullet_Include.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Types.hpp"

#include "Common/Resource/Assimp_Include.hpp"

class ConcaveMeshShape : public CollisionShape
{
public:
	ConcaveMeshShape(string fileName)
	{
		Assimp::Importer import;
		const aiScene *scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

		btTriangleMesh* triMesh = nullptr;

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			//cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		}

		if (scene->HasMeshes())
		{
			//Only Support 1 mesh for now
			aiMesh* mesh = scene->mMeshes[0];

			vector<vector3D> vertices;
			vector<unsigned int> indices;

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				vector3D vertex;

				//Need to swap Y and Z while loading
				vertex.x = mesh->mVertices[i].x;
				vertex.y = mesh->mVertices[i].y;
				vertex.z = mesh->mVertices[i].z;

				vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}


			triMesh = new btTriangleMesh();
			for (unsigned int i = 0; i < vertices.size(); i += 3)
			{
				triMesh->addTriangle(toBtVec3(vertices[i + 0]), toBtVec3(vertices[i + 1]), toBtVec3(vertices[i + 2]), true);
			}
		}

		import.FreeScene();

		if (triMesh != nullptr)
		{
			this->btShape = new btBvhTriangleMeshShape(triMesh, true);
		}
	};

};

#endif //CONCAVEMESHSHAPE_HPP