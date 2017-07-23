#include "Client/Resource/TexturedMesh.hpp"
#include "Client/Rendering/ShaderProgram.hpp"

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
		GL_FALSE,           // normalized?
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

TexturedMesh* TexturedMesh::loadObj(std::string filename)
{
	FILE* file;
	errno_t err = fopen_s(&file, filename.c_str(), "r");
	if (file == NULL || err != 0)
	{
		printf("Cannot Open File: %s \n", filename.c_str());
		return nullptr;
	}

	std::vector< vector3F > temp_vertices;
	std::vector< vector2F > temp_uvs;
	std::vector< vector3F > temp_normals;

	//Push Back the zero pos since we use 1 as the start point
	temp_vertices.push_back(vector3F(0.0));
	temp_uvs.push_back(vector2F(0.0));
	temp_normals.push_back(vector3F(0.0));

	std::vector<TexturedVertex> vertices;
	std::vector<unsigned int> indices;

	while (1)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0)
		{
			vector3F vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vector2F uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vector3F normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				printf("Error: unsupported .obj file\n");
				printf("File: %s\n\n", filename.c_str());
			}

			vertices.push_back({ temp_vertices[vertexIndex[0]], temp_normals[normalIndex[0]], temp_uvs[uvIndex[0]] });
			vertices.push_back({ temp_vertices[vertexIndex[1]], temp_normals[normalIndex[1]], temp_uvs[uvIndex[1]] });
			vertices.push_back({ temp_vertices[vertexIndex[2]], temp_normals[normalIndex[2]], temp_uvs[uvIndex[2]] });
			indices.push_back((unsigned int)indices.size());
			indices.push_back((unsigned int)indices.size());
			indices.push_back((unsigned int)indices.size());
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	return new TexturedMesh(vertices, indices);
}