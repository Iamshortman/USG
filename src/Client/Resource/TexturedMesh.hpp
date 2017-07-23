#ifndef TEXTUREDMESH_HPP
#define TEXTUREDMESH_HPP

#include "Client/Rendering/OpenGL_Include.hpp"
#include "Common/GLM_Include.hpp"
#include "Common/Resource/Mesh.hpp"
#include <vector>

struct TexturedVertex
{
	vector3F pos;
	vector3F normal;
	vector2F uv;
};

class TexturedMesh : public Mesh
{
public:
	TexturedMesh(std::vector<TexturedVertex>& vertices, std::vector<unsigned int>& indices);
	virtual ~TexturedMesh();

	virtual void draw(ShaderProgram* program);

	static TexturedMesh* loadObj(std::string filename);

private:
	GLuint vbo;
	GLuint ibo;

	int size;
};

#endif //TEXTUREDMESH_HPP
