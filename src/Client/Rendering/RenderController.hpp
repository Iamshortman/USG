#ifndef RENDERCONTROLLER_HPP
#define RENDERCONTROLLER_HPP

#include "Client/Rendering/ShaderProgram.hpp"

#include "Common/Entity/Entity.hpp"

class RenderController
{
public:
	virtual void update(double deltaTime) = 0;
	virtual void setGLSLUniform(ShaderProgram* shader) = 0;

private:
	Entity* parent;
};

#endif //RENDERCONTROLLER_HPP