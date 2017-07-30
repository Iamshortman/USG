#ifndef LIGHTSHADERUTIL_HPP
#define LIGHTSHADERUTIL_HPP

#include "Client/Rendering/Lights.hpp"
#include "Client/Rendering/ShaderProgram.hpp"

#include <string>

void setBaseLight(std::string prefix, ShaderProgram* program, BaseLight* light)
{
	program->setUniform(prefix + ".color", light->getColor());
	program->setUniform(prefix + ".intensity", light->getIntensity());
};

void setDirectionalLight(std::string prefix, ShaderProgram* program, DirectionalLight* light, Transform worldTransform)
{
	setBaseLight(prefix + ".base", program, light);
	program->setUniform(prefix + ".direction", ((quaternionF)worldTransform.getOrientation()) * light->getDirection());
};

void setPointLight(std::string prefix, ShaderProgram* program, PointLight* light, Transform worldTransform, vector3D camPos)
{
	vector3D pos = worldTransform.getPosition() + (worldTransform.getOrientation() * light->getPosition());

	setBaseLight(prefix + ".base", program, light);
	program->setUniform(prefix + ".atten", light->getAttenuation());
	program->setUniform(prefix + ".position", (vector3F)(pos - camPos));
	program->setUniform(prefix + ".range", light->getRange());
};

void setSpotLight(std::string prefix, ShaderProgram* program, SpotLight* light, Transform worldTransform, vector3D camPos)
{
	setPointLight(prefix + ".point", program, light, worldTransform, camPos);
	program->setUniform(prefix + ".direction", ((quaternionF)worldTransform.getOrientation()) * light->getDirection());
	program->setUniform(prefix + ".cutoff", light->getCutoff());
};

#endif //LIGHTSHADERUTIL_HPP