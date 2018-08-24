#include "ShadowMap.hpp"

#include "Common/Logger/Logger.hpp"

ShadowMap::ShadowMap(vector2I buffer_size)
{
	this->size = buffer_size;

	glGenFramebuffers(1, &this->frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);

	glGenTextures(1, &this->shadow_texture);
	glBindTexture(GL_TEXTURE_2D, this->shadow_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, this->size.x, this->size.y);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->size.x, this->size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->shadow_texture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum fbo_error = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (fbo_error != GL_FRAMEBUFFER_COMPLETE)
	{
		Logger::getInstance()->logError("FrameBuffer not Complete: %x\n", fbo_error);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMap::~ShadowMap()
{
	glDeleteTextures(1, &this->shadow_texture);

	glDeleteFramebuffers(1, &this->frame_buffer);
}

GLuint ShadowMap::getFBO()
{
	return this->frame_buffer;
}

vector2I ShadowMap::getBufferSize()
{
	return this->size;
}

void ShadowMap::clearBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);
	glClearDepth(0.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
}
