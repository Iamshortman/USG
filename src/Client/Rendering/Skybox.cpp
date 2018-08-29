#include "Skybox.hpp"

#include "Client/Resource/ShaderPool.hpp"
#include "Common/Logger/Logger.hpp"
#include "Client/Resource/TexturePool.hpp"


Skybox::Skybox(string cube_path_base, string shader_name)
{
	this->shader_program = new ShaderProgram(shader_name + ".vs", shader_name + ".fs");

	string postfixes[] = { "_right.png", "_left.png", "_top.png", "_bottom.png", "_front.png", "_back.png" };

	glGenTextures(1, &this->cube_map);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cube_map);

	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, numComponents;
		unsigned char* data = load_png((cube_path_base + postfixes[i]).c_str(), &width, &height, &numComponents, 4);

		if (data == NULL)
		{
			Logger::getInstance()->logError("Can't Load the given texture: %s\n", (cube_path_base + postfixes[i]).c_str());
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		unload_png(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	this->cube_mesh = TexturedMesh::loadObj("res/models/skybox.obj");
}

Skybox::~Skybox()
{
	delete this->shader_program;
	delete this->cube_mesh;

	glDeleteTextures(1, &this->cube_map);

}

void Skybox::draw(Camera* camera, Transform& camera_transform, vector2I size)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cube_map);

	this->shader_program->setActiveProgram();
	
	this->shader_program->setUniform("projection", camera->getProjectionMatrix(size));
	this->shader_program->setUniform("view", camera_transform.getOriginViewMatrix());

	this->cube_mesh->draw(this->shader_program);
}
