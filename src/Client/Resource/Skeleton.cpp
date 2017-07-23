#include "Client/Resource/Skeleton.hpp"
#include "Common/Resource/Assimp_Include.hpp"

#include <hash_map>

Bone* generateSkeleton(aiNode* node, std::hash_map<string, Bone*>* map)
{
	if (node == nullptr)
	{
		return nullptr;
	}

	Bone* bone = map->at(node->mName.data);

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		Bone* child = generateSkeleton(node->mChildren[i], map);

		if (child != nullptr)
		{
			bone->children.push_back(child);
			child->parent_bone = bone;
		}
	}

	return bone;
}

Skeleton::Skeleton(Bone* root, int count)
	: rootBone(root), boneCount(count)
{
	rootBone->calcInverseBindTransform(matrix4());
}

Skeleton* Skeleton::loadSkeleton(std::string filename)
{
	Skeleton* skeleton = nullptr;

	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		//cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return nullptr;
	}

	if (scene->HasMeshes())
	{
		//Only Support 1 mesh for now
		aiMesh* mesh = scene->mMeshes[0];

		std::hash_map<string, Bone*> map;

		if (mesh->HasBones())
		{
			for (unsigned int i = 0; i < mesh->mNumBones; i++)
			{
				aiBone* bone = mesh->mBones[i];
				string name = string(bone->mName.data);
				//matrix4 offset = glm::transpose(toMat4(bone->mOffsetMatrix));
				aiNode* node = scene->mRootNode->FindNode(bone->mName);
				if (node == nullptr)
				{
					return nullptr;
				}

				matrix4 offset = toMat4(bone->mOffsetMatrix);

				map[name] = new Bone(name, offset);
			}
		}

		aiNode* armature = scene->mRootNode->FindNode("Armature");
		if (armature == nullptr)
		{
			return nullptr;
		}

		Bone* root = generateSkeleton(armature->mChildren[0]->mChildren[0], &map);

		skeleton = new Skeleton(root, map.size());
	}

	import.FreeScene();

	return skeleton;
}