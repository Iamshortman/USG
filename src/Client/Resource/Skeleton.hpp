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

	matrix4 animatedTransform;
	matrix4 localBindTransform;
	matrix4 inverseBindTransform;

	Bone::Bone(std::string in_name, matrix4 transform)
	{
		this->name = in_name;
		this->localBindTransform = transform;
		parent_bone = nullptr;
	};

	void calcInverseBindTransform(matrix4 parentBindTransform) 
	{
		matrix4 bindTransform = parentBindTransform * localBindTransform;
		matrix4 inverseBindTransform = glm::inverse(bindTransform);
		for (Bone* child : children) 
		{
			child->calcInverseBindTransform(bindTransform);
		}
	}

};

class Skeleton
{
public:
	Bone* rootBone = nullptr;
	int boneCount = 0;

	Skeleton(Bone* root, int count);

	static Skeleton* loadSkeleton(std::string filename);

private:

};

#endif //SKELETON_HPP