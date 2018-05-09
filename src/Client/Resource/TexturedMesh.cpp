#include "Client/Resource/TexturedMesh.hpp"
#include "Client/Rendering/ShaderProgram.hpp"

#include "Common/Resource/Assimp_Include.hpp"

TexturedMesh::TexturedMesh(std::vector<TexturedVertex>& vertices, std::vector<unsigned int>& indices)
{
	size = (int)indices.size();

	//GenBuffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	//Adds the data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TexturedVertex), &vertices[0], GL_STATIC_DRAW);

	//Adds the indices to the buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

TexturedMesh::~TexturedMesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void TexturedMesh::draw(ShaderProgram* program)
{
	if (size == 0)
	{
		return;
	}

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
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

	//Disable Attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

TexturedMesh* TexturedMesh::loadObj(std::string fileName)
{
	TexturedMesh* textMesh = nullptr;

	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return textMesh;
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

				//Need to swap Y and Z while loading
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

			unsigned int indicesOffset = (unsigned int) indices.size();

			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(indicesOffset + face.mIndices[j]);
			}
		}

		textMesh = new TexturedMesh(vertices, indices);
	}

	import.FreeScene();

	return textMesh;
}