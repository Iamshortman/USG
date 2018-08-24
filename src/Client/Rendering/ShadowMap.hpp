#ifndef SHADOW_MAP_HPP
#define SHADOW_MAP_HPP

#include "Client/Rendering/FBO.hpp"
#include "Client/Rendering/OpenGL_Include.hpp"
#include "Common/GLM_Include.hpp"

class ShadowMap : public FBO
{
public:
	ShadowMap(vector2I buffer_size);
	~ShadowMap();

	virtual GLuint getFBO();
	virtual vector2I getBufferSize();
	virtual void clearBuffer();

	GLuint getShadowTexture() { return this->shadow_texture; };

private:
	GLuint frame_buffer;
	GLuint shadow_texture;

	vector2I size;
};

#endif //SHADOW_MAP_HPP