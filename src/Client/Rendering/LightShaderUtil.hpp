#include "Client/Rendering/Lights.hpp"
#include "Client/Rendering/ShaderProgram.hpp"

#include <string>

void setBaseLight(std::string prefix, ShaderProgram* program, BaseLight* light)
{
	program->setUniform(prefix + ".color", light->getColor());
	program->setUniform(prefix + ".intensity", light->getIntensity());
};

void setDirectionalLight(std::string prefix, ShaderProgram* program, DirectionalLight* light)
{
	setBaseLight(prefix + ".base", program, light);
	program->setUniform(prefix + ".direction", light->getDirection());
};

void setPointLight(std::string prefix, ShaderProgram* program, PointLight* light, vector3D camPos)
{
	setBaseLight(prefix + ".base", program, light);
	program->setUniform(prefix + ".atten", light->getAttenuation());
	program->setUniform(prefix + ".position", (vector3F) (camPos - light->getPosition()));
	program->setUniform(prefix + ".range", light->getRange());
};

void setSpotLight(std::string prefix, ShaderProgram* program, SpotLight* light, vector3D camPos)
{
	setPointLight(prefix + ".point", program, light, camPos);
	program->setUniform(prefix + ".direction", light->getDirection());
	program->setUniform(prefix + ".cutoff", light->getCutoff());
};
