#ifndef G_BUFFER_HPP
#define G_BUFFER_HPP

#include "Client/Rendering/OpenGL_Include.hpp"

class G_Buffer
{
public:
	G_Buffer(int width, int height);
	~G_Buffer();

	GLuint getFBO();
	void getBufferSize(int &width, int &height);

	void clearBuffer();

private:
	GLuint frame_buffer;
	GLuint position_texture;
	GLuint normal_texture;
	GLuint color_texture;
	GLuint depth_texture;
	int width;
	int height;
};

#endif //G_BUFFER_HPP