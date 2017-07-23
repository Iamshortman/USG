#ifndef ANIMATEDMESH_HPP
#define ANIMATEDMESH_HPP

#include "Common/Types.hpp"
#include "Client/Rendering/OpenGL_Include.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Resource/Mesh.hpp"
#include <vector>
#include <hash_map>

struct AnimatedVertex
{
	vector3F pos;
	vector3F normal;
	vector2F uv;
	vector3I boneIndices;
	vector3F boneWeights;
};

class AnimatedMesh : public Mesh
{
public:
	AnimatedMesh(std::vector<AnimatedVertex>& vertices, std::vector<unsigned int>& indices, std::hash_map<string, unsigned int> map);
	virtual ~AnimatedMesh();

	virtual void draw(ShaderProgram* program);

	static AnimatedMesh* loadMesh(string fileName);

	std::hash_map<string, unsigned int> boneMap;

private:

	GLuint vbo;
	GLuint ibo;

	int size;
};

#endif //ANIMATEDMESH_HPP