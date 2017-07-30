#include "Client/Resource/AnimatedMesh.hpp"
#include "Client/Rendering/ShaderProgram.hpp"

#include "Common/Resource/Assimp_Include.hpp"

AnimatedMesh::AnimatedMesh(std::vector<AnimatedVertex>& vertices, std::vector<unsigned int>& indices, std::hash_map<string, unsigned int> map)
{
	size = (int)indices.size();

	//GenBuffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	//Adds the data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(AnimatedVertex), &vertices[0], GL_STATIC_DRAW);

	//Adds the indices to the buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	this->boneMap = map;
}

AnimatedMesh::~AnimatedMesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void AnimatedMesh::draw(ShaderProgram* program)
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
		sizeof(AnimatedVertex),     // stride
		(void*)0            // array buffer offset
		);

	//Normal
	glVertexAttribPointer(
		1,                  // Normal attribute location
		3,					// size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(AnimatedVertex),     // stride
		(void*)offsetof(AnimatedVertex, normal) // array buffer offset
		);

	//UV
	glVertexAttribPointer(
		2,                  // UV attribute location
		2,					// uv is a vec2
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(AnimatedVertex),     // stride
		(void*)offsetof(AnimatedVertex, uv) // array buffer offset
		);

	//JointIndices
	glVertexAttribPointer(
		3,                  // jointIndices attribute location
		3,					// jointIndices is a vec3
		GL_INT,           // type
		GL_FALSE,           // normalized?
		sizeof(AnimatedVertex),     // stride
		(void*)offsetof(AnimatedVertex, boneIndices) // array buffer offset
		);

	//JointWeights
	glVertexAttribPointer(
		1,                  // jointWeights attribute location
		3,					// size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(AnimatedVertex),     // stride
		(void*)offsetof(AnimatedVertex, boneWeights) // array buffer offset
		);


	//Draw the mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

	//Disable Attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

AnimatedMesh* AnimatedMesh::loadMesh(string fileName)
{
	AnimatedMesh* animMesh = nullptr;

	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return animMesh;
	}

	if (scene->HasMeshes())
	{
		//Only Support 1 mesh for now
		aiMesh* mesh = scene->mMeshes[0];

		vector<AnimatedVertex> vertices;
		vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			AnimatedVertex vertex;

			//Need to swap Y and Z while loading
			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].z;
			vertex.pos.z = mesh->mVertices[i].y;

			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].z;
			vertex.normal.z = mesh->mNormals[i].y;

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				vertex.uv.x = mesh->mTextureCoords[0][i].x;
				vertex.uv.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.uv = vector2F(0.0f);
			}

			//Setup default values
			vertex.boneIndices = vector3I(-1);
			vertex.boneWeights = vector3F(0.0f);

			vertices.push_back(vertex);
		}

		std::hash_map<string, unsigned int> boneMap;

		if (mesh->HasBones())
		{
			for (unsigned int i = 0; i < mesh->mNumBones; i++)
			{
				aiBone* bone = mesh->mBones[i];
				string name = string(bone->mName.data);
				boneMap[name] = i;

				for (unsigned int j = 0; j < bone->mNumWeights; j++)
				{
					unsigned int index = bone->mWeights[j].mVertexId;
					float weight = bone->mWeights[j].mWeight;
					AnimatedVertex* vertex = &vertices[index];
					
					vertex->boneIndices.x = i;
					vertex->boneWeights.x = 1.0f;
					/*if (weight > vertex->boneWeights.x)
					{
						//Shift y to z
						vertex->boneIndices.z = vertex->boneIndices.y;
						vertex->boneWeights.z = vertex->boneWeights.y;
						
						//Shift x to y
						vertex->boneIndices.y = vertex->boneIndices.x;
						vertex->boneWeights.y = vertex->boneWeights.x;

						//Insert new into weight into x
						vertex->boneIndices.x = i;
						vertex->boneWeights.x = weight;
					}
					else if (weight > vertex->boneWeights.y)
					{
						//Shift y to z
						vertex->boneIndices.z = vertex->boneIndices.y;
						vertex->boneWeights.z = vertex->boneWeights.y;

						//Insert new into weight into y
						vertex->boneIndices.y = i;
						vertex->boneWeights.y = weight;
					}
					else if(weight > vertex->boneWeights.z)
					{
						//Insert new into weight into z
						vertex->boneIndices.z = i;
						vertex->boneWeights.z = weight;
					}*/

				}
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		animMesh = new AnimatedMesh(vertices, indices, boneMap);
	}

	import.FreeScene();

	return animMesh;
}