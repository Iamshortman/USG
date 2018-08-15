#ifndef G_BUFFER_HPP
#define G_BUFFER_HPP

#include "Client/Rendering/FBO.hpp"
#include "Client/Rendering/OpenGL_Include.hpp"
#include "Common/GLM_Include.hpp"

class G_Buffer : public FBO
{
public:
	G_Buffer(vector2I buffer_size, bool multisample = false, int number_of_samples = 0);
	~G_Buffer();

	virtual GLuint getFBO();
	virtual vector2I getBufferSize();
	virtual void clearBuffer();

	GLuint getAlbedoTexture() { return this->albedo_texture; };
	GLuint getNormalTexture() { return this->normal_texture; };
	GLuint getPositionTexture() { return this->position_texture; };
	GLuint getDepthTexture() { return this->depth_texture; };

private:
	GLuint frame_buffer;
	GLuint albedo_texture;
	GLuint normal_texture;
	GLuint position_texture;
	GLuint depth_texture;

	vector2I size;
	bool multisample;
	int number_of_samples;
};

#endif //G_BUFFER_HPP