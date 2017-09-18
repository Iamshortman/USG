#ifndef SKELETON_HPP
#define SKELETON_HPP

#include "Common/Types.hpp"
#include "Client/Rendering/OpenGL_Include.hpp"
#include "Common/GLM_Include.hpp"

class Bone
{
public:
	string name;
	Bone* parent_bone;    //A pointer to this bone's parent bone.
	std::vector<Bone*> children;

	matrix4 offsetTransform;

	Bone::Bone(std::string in_name, matrix4 transform)
	{
		this->name = in_name;
		this->offsetTransform = transform;
		parent_bone = nullptr;
	};


	matrix4 getAnimatedMatrix()
	{
		if (this->parent_bone != nullptr)
		{
			return this->parent_bone->getAnimatedMatrix() * this->offsetTransform;
		}

		return this->offsetTransform;
	}
};

class Skeleton
{
public:
	Bone* rootBone = nullptr;
	size_t boneCount = 0;

	Skeleton(Bone* root, size_t count);

	static Skeleton* loadSkeleton(std::string filename);

private:

};

#endif //SKELETON_HPP