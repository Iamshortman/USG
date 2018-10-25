#ifndef TEXTURED_MESH_INSTANCED_HPP
#define TEXTURED_MESH_INSTANCED_HPP

#include "Client/Resource/TexturedMesh.hpp"
#include <vector>

class TexturedMeshInstanced : public TexturedMesh
{
public:
	TexturedMeshInstanced(std::vector<TexturedVertex>& vertices, std::vector<unsigned int>& indices);
	virtual ~TexturedMeshInstanced();

	void draw(std::vector<matrix4> model_matrices);

	static TexturedMeshInstanced* loadObj(std::string file_name);

private:
};

#endif //TEXTURED_MESH_INSTANCED_HPP
