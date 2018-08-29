#ifndef FBO_HPP
#define FBO_HPP

#include "Client/Rendering/OpenGL_Include.hpp"
#include "Common/GLM_Include.hpp"

class FBO
{
	virtual GLuint getFBO() = 0;
	virtual vector2I getBufferSize() = 0;
	virtual void clearBuffer() = 0;
};

#endif //FBO_HPP