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
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtx/string_cast.hpp>

typedef glm::mat3 matrix3;
typedef glm::mat4 matrix4;

typedef glm::f64mat3 matrix3D;
typedef glm::f64mat4 matrix4D;

typedef glm::f32vec2 vector2F;
typedef glm::f32vec3 vector3F;
typedef glm::f32vec4 vector4F;
typedef glm::quat quaternionF;

typedef glm::f64vec2 vector2D;
typedef glm::f64vec3 vector3D;
typedef glm::f64vec4 vector4D;
typedef glm::f64quat quaternionD;

typedef glm::i8vec3 vector3B;
typedef glm::i32vec3 vector3I;
typedef glm::i32vec2 vector2I;

#endif //GLM_INCLUDE_HPP
