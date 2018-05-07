#include "G_Buffer.hpp"

G_Buffer::G_Buffer(int width, int height)
{
	this->width = width;
	this->height = height;

	glGenFramebuffers(1, &this->frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);

	//position color buffer
	glGenTextures(1, &this->position_texture);
	glBindTexture(GL_TEXTURE_2D, this->position_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->position_texture, 0);

	//normal color buffer
	glGenTextures(1, &this->normal_texture);
	glBindTexture(GL_TEXTURE_2D, this->normal_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, width, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->normal_texture, 0);

	// - color + specular color buffer
	glGenTextures(1, &this->color_texture);
	glBindTexture(GL_TEXTURE_2D, this->color_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->color_texture, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glGenTextures(1, &this->depth_texture);
	glBindTexture(GL_TEXTURE_2D, this->depth_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depth_texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



G_Buffer::~G_Buffer()
{
	glDeleteTextures(1, &this->position_texture);
	glDeleteTextures(1, &this->normal_texture);
	glDeleteTextures(1, &this->color_texture);
	glDeleteTextures(1, &this->depth_texture);

	glDeleteFramebuffers(1, &this->frame_buffer);
}

GLuint G_Buffer::getFBO()
{
	return this->frame_buffer;
}

void G_Buffer::getBufferSize(int& width, int& height)
{
	width = this->width;
	height = this->height;
}

void G_Buffer::clearBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
