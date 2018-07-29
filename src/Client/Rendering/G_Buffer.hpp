#ifndef G_BUFFER_HPP
#define G_BUFFER_HPP

#include "Client/Rendering/OpenGL_Include.hpp"
#include "Common/GLM_Include.hpp"

class G_Buffer
{
public:
	G_Buffer(vector2I buffer_size, bool multisample = false, int number_of_samples = 0);
	~G_Buffer();

	GLuint getFBO();
	vector2I getBufferSize();

	void clearBuffer();

	GLuint getPositionTexture() { return this->position_texture; };
	GLuint getNormalTexture() { return this->normal_texture; };
	GLuint getAlbedoTexture() { return this->albedo_texture; };
	GLuint getDepthTexture() { return this->depth_texture; };

private:
	GLuint frame_buffer;
	GLuint position_texture;
	GLuint normal_texture;
	GLuint albedo_texture;
	GLuint depth_texture;

	vector2I size;
	bool multisample;
	int number_of_samples;
};

#endif //G_BUFFER_HPP