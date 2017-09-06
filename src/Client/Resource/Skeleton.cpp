#include "Client/Resource/Skeleton.hpp"
#include "Common/Resource/Assimp_Include.hpp"

#include <unordered_map>
#include <vector>

Bone* generateSkeleton(aiNode* node, std::unordered_map<string, Bone*>* map)
{
	/*if (node == nullptr)
	{
		return nullptr;
	}

	string name = string(node->mName.data);

	if (map->find(name) != map->end())
	{
		return nullptr;
	}

	Bone* bone = map->at(node->mName.data);

	aiMatrix4x4 tempMat = node->mTransformation;
	bone->animatedTransform = toMat4(tempMat.Transpose());

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		Bone* child = generateSkeleton(node->mChildren[i], map);

		if (child != nullptr)
		{
			bone->children.push_back(child);
			child->parent_bone = bone;
		}
	}*/

	return nullptr;
}

Skeleton::Skeleton(Bone* root, int count)
	: rootBone(root), boneCount(count)
{
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

		std::unordered_map<string, Bone*> map;

		if (mesh->HasBones())
		{
			for (unsigned int i = 0; i < mesh->mNumBones; i++)
			{
				aiBone* bone = mesh->mBones[i];
				string name = string(bone->mName.data);

				if (name == "" || bone == nullptr)
				{
					printf("Error: Can't load bone!\n");
					return nullptr;
				}

				matrix4 offset = toMat4(bone->mOffsetMatrix);

				offset = glm::inverse(offset);

				for (int i = 0; i < 4; i++)
				{
					printf("[");
					for (int j = 0; j < 4; j++)
					{
						printf(" %f", offset[i][j]);
					}
					printf(" ]\n");
				}
				printf("\n");

				//offset = glm::transpose(offset);
				//offset = glm::inverse(offset);

				//offset = glm::translate(matrix4(1.0f), vector3F(0.0f, 2.0f, 0.0f));

				map[name] = new Bone(name, offset);
			}
		}

		if (map.size() == 0)
		{
			return nullptr;
		}

		//Hierarchy Loading
		for (std::pair<string, Bone*> bone : map)
		{
			aiNode* node = scene->mRootNode->FindNode(bone.first.c_str());

			if (node != nullptr)
			{
				aiNode* child;
				Bone* childBone;
				for (int i = 0; i < node->mNumChildren; i++)
				{
					child = node->mChildren[i];

					if (child == nullptr)
					{
						printf("Error: Bone not found!!!!\n");
					}

					if (map.find(child->mName.data) != map.end())
					{
						childBone = map[child->mName.data];
						bone.second->children.push_back(childBone);
						childBone->parent_bone = bone.second;
					}

					//childBone->offsetTransform = toMat4(child->mTransformation);
				}
			}
		}

		vector<Bone*> rootBones;
		for (std::pair<string, Bone*> bone : map)
		{
			//Look for root bone
			if (bone.second->parent_bone == nullptr)
			{
				rootBones.push_back(bone.second);
			}
		}

		Bone* root;

		//Create a new root if there is more then 1 root;
		if (rootBones.size() > 1)
		{
			root = new Bone("NewRoot", matrix4(1.0f));

			for (Bone* bone : rootBones)
			{
				root->children.push_back(bone);
				bone->parent_bone = root;
			}

		}
		else
		{
			root = rootBones[0];
		}

		if (root == nullptr)
		{
			return nullptr;
		}

		skeleton = new Skeleton(root, map.size());
	}

	import.FreeScene();

	return skeleton;
}