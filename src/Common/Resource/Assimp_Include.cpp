#include "Common/Resource/Assimp_Include.hpp"

aiMatrix4x4 toAiMat4(matrix4 mat)
{
	return aiMatrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

matrix4 toMat4(aiMatrix4x4& in_mat)
{
	glm::mat4 to;

	to[0][0] = in_mat.a1; to[0][1] = in_mat.b1;  to[0][2] = in_mat.c1; to[0][3] = in_mat.d1;
	to[1][0] = in_mat.a2; to[1][1] = in_mat.b2;  to[1][2] = in_mat.c2; to[1][3] = in_mat.d2;
	to[2][0] = in_mat.a3; to[2][1] = in_mat.b3;  to[2][2] = in_mat.c3; to[2][3] = in_mat.d3;
	to[3][0] = in_mat.a4; to[3][1] = in_mat.b4;  to[3][2] = in_mat.c4; to[3][3] = in_mat.d4;

	return to;
}