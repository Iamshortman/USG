#ifndef GLM_INCLUDE_HPP
#define GLM_INCLUDE_HPP

//Used to include GLM in correct order
#include <stdlib.h>
#include <iostream>

//GML needs this to not complain about rotations being deprecated
#define GLM_FORCE_RADIANS

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288   /* pi */
#endif // !M_PI

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

typedef glm::mat4 matrix4;
typedef glm::mat3 matrix3;

typedef glm::vec4 vector4F;
typedef glm::vec3 vector3F;
typedef glm::vec2 vector2F;
typedef glm::quat quaternionF;

typedef glm::f64vec3 vector3D;
typedef glm::f64quat quaternionD;

typedef glm::i8vec3 vector3B;
typedef glm::i32vec3 vector3I;
typedef glm::i32vec2 vector2I;

struct HashVector3B
{
	size_t operator()(const vector3B& k)const
	{
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
	}

	bool operator()(const vector3B& a, const vector3B& b)const
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

#endif //GLM_INCLUDE_HPP
