#ifndef MODEL_HPP
#define MODEL_HPP

#include "Common/Resource/Mesh.hpp"
#include "Client/Rendering/ShaderProgram.hpp"
#include "Client/Rendering/RenderController.hpp"
#include "Client/Resource/Skeleton.hpp"

class RenderController;

class Model
{
public:
	Model();
	~Model();
	void setMesh(std::string name);
	void setShader(std::string name);
	void setLightingShader(std::string name);
	void setController(RenderController* renderController);
	void addTexture(std::string name, int slot);

	Mesh* getMesh();
	ShaderProgram* getShader();
	ShaderProgram* getLightingShader();
	RenderController* getController();
	GLuint getTexture(int slot);

	Skeleton* skeleton = nullptr;

private:
	Mesh* mesh = nullptr;
	std::string meshString;

	ShaderProgram* program = nullptr;
	std::string programString;

	ShaderProgram* lightingProgram = nullptr;
	std::string lightingProgramString;
	
	RenderController* controller = nullptr;

	std::string texturesString[10];
	GLuint textures[10];
};

#endif //MODEL_HPP