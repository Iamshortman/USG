#ifndef ASSIMP_INCLUDE_HPP
#define ASSIMP_INCLUDE_HPP

#include "Common/GLM_Include.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

aiMatrix4x4 toAiMat4(matrix4 mat);
matrix4 toMat4(aiMatrix4x4& in_mat);

#endif //ASSIMP_INCLUDE_HPP