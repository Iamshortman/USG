#include "TexturedMeshInstanced.hpp"

#include "Client/Rendering/ShaderProgram.hpp"
#include "Common/Resource/Assimp_Include.hpp"
#include "Common/Logger/Logger.hpp"

TexturedMeshInstanced::TexturedMeshInstanced(std::vector<TexturedVertex>& vertices, std::vector<unsigned int>& indices)
	: TexturedMesh(vertices, indices)
{

}

TexturedMeshInstanced::~TexturedMeshInstanced()
{
	
}

void TexturedMeshInstanced::draw(std::vector<matrix4> model_matrices)
{
	GLuint matrices_buffer;
	glGenBuffers(1, &matrices_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, matrices_buffer);
	glBufferData(GL_ARRAY_BUFFER, model_matrices.size() * sizeof(glm::mat4), &model_matrices[0], GL_STATIC_DRAW);

	//Enable Attributes
	glEnableVertexAttribArray(3); //ModelMatrix1
	glEnableVertexAttribArray(4); //ModelMatrix2
	glEnableVertexAttribArray(5); //ModelMatrix3
	glEnableVertexAttribArray(6); //ModelMatrix4
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vector4F), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vector4F), (void*)(sizeof(vector4F)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vector4F), (void*)(2 * sizeof(vector4F)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vector4F), (void*)(3 * sizeof(vector4F)));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	
	//Enable Attributes
	glEnableVertexAttribArray(0); //Position
	glEnableVertexAttribArray(1); //Normal
	glEnableVertexAttribArray(2); //UV

	//Bind the buffer to begin drawing
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Position
	glVertexAttribPointer(
		0,                  // Position attribute location
		3,					// size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(TexturedVertex),     // stride
		(void*)0            // array buffer offset
	);

	//Normal
	glVertexAttribPointer(
		1,                  // Normal attribute location
		3,					// size
		GL_FLOAT,           // type
		GL_TRUE,           // normalized?
		sizeof(TexturedVertex),     // stride
		(void*)offsetof(TexturedVertex, normal) // array buffer offset
	);

	//UV
	glVertexAttribPointer(
		2,                  // UV attribute location
		2,					// uv is a vec2
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(TexturedVertex),     // stride
		(void*)offsetof(TexturedVertex, uv) // array buffer offset
	);

	//Draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElementsInstanced(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0, model_matrices.size());

	//Disable Attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);

	glDeleteBuffers(1, &matrices_buffer);
}

TexturedMeshInstanced* TexturedMeshInstanced::loadObj(std::string file_name)
{
	TexturedMeshInstanced* texture_mesh = nullptr;

	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(file_name, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Logger::getInstance()->logError("Error ASSIMP: %s\n", import.GetErrorString());
		return texture_mesh;
	}

	if (scene->HasMeshes())
	{
		vector<TexturedVertex> vertices;
		vector<unsigned int> indices;

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				TexturedVertex vertex;

				vertex.pos.x = mesh->mVertices[i].x;
				vertex.pos.y = mesh->mVertices[i].y;
				vertex.pos.z = mesh->mVertices[i].z;

				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;

				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					vertex.uv.x = mesh->mTextureCoords[0][i].x;
					vertex.uv.y = mesh->mTextureCoords[0][i].y;
				}
				else
				{
					vertex.uv = vector2F(0.0f);
				}

				vertices.push_back(vertex);
			}

			unsigned int indicesOffset = (unsigned int)indices.size();

			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(indicesOffset + face.mIndices[j]);
			}
		}

		texture_mesh = new TexturedMeshInstanced(vertices, indices);
	}

	import.FreeScene();

	return texture_mesh;
}
