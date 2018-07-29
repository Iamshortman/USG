#include "RenderingSystem.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"

RenderingSystem::RenderingSystem()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_2D);

	vector<TexturedVertex> vertices;
	vertices.push_back({ vector3F(-1.0f,  1.0f, 0.5f), vector3F(0.0f),  vector2F(0.0f, 1.0f) });
	vertices.push_back({ vector3F(-1.0f, -1.0f, 0.5f), vector3F(0.0f),  vector2F(0.0f, 0.0f) });
	vertices.push_back({ vector3F(1.0f,  1.0f, 0.5f), vector3F(0.0f),  vector2F(1.0f, 1.0f) });
	vertices.push_back({ vector3F(1.0f, -1.0f, 0.5f), vector3F(0.0f),  vector2F(1.0f, 0.0f) });

	vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	this->full_screen_quad = new TexturedMesh(vertices, indices);
	this->full_screen_quad_program = new ShaderProgram("res/shaders/ScreenTexture.vs", "res/shaders/ScreenTexture.fs");
}

RenderingSystem::~RenderingSystem()
{
	delete this->full_screen_quad;
	delete this->full_screen_quad_program;
}

void RenderingSystem::renderMS(GLuint render_target, G_Buffer* ms_g_buffer, G_Buffer* g_buffer, Camera* camera)
{
	vector2I ms_buffer_size = ms_g_buffer->getBufferSize();
	vector2I g_buffer_size = g_buffer->getBufferSize();

	//TODO global clear buffer fuctions
	glBindFramebuffer(GL_FRAMEBUFFER, ms_g_buffer->getFBO());
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (this->skybox != nullptr)
	{
		skybox->draw(camera, ms_buffer_size.x, ms_buffer_size.y);

		glClearDepth(0.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	//DRAW ALL MESHES
	for (int i = 0; i < this->models.size(); i++)
	{
		Mesh* mesh = MeshPool::getInstance()->get(this->models[i].first->getMesh());
		GLuint texture = TexturePool::getInstance()->get(this->models[i].first->getTexture());
		ShaderProgram* program = ShaderPool::getInstance()->get(this->models[i].first->getAmbientShader());

		this->RenderModel(mesh, texture, program, this->models[i].second, camera, ms_buffer_size);
	}

	//Blit all color attachments + depth
	glBindFramebuffer(GL_READ_FRAMEBUFFER, ms_g_buffer->getFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_buffer->getFBO());

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(
		0, 0, ms_buffer_size.x, ms_buffer_size.y,
		0, 0, g_buffer_size.x, g_buffer_size.y,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(
		0, 0, ms_buffer_size.x, ms_buffer_size.y,
		0, 0, g_buffer_size.x, g_buffer_size.y,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glBlitFramebuffer(
		0, 0, ms_buffer_size.x, ms_buffer_size.y,
		0, 0, g_buffer_size.x, g_buffer_size.y,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBlitFramebuffer(
		0, 0, ms_buffer_size.x, ms_buffer_size.y,
		0, 0, g_buffer_size.x, g_buffer_size.y,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, render_target);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_buffer->getPositionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_buffer->getNormalTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, g_buffer->getAlbedoTexture());

	this->full_screen_quad_program->setActiveProgram();
	this->full_screen_quad_program->setUniform("gPosition", 0);
	this->full_screen_quad_program->setUniform("gNormal", 1);
	this->full_screen_quad_program->setUniform("gAlbedoSpec", 2);

	this->full_screen_quad->draw(this->full_screen_quad_program);

	this->full_screen_quad_program->deactivateProgram();
}

void RenderingSystem::addModel(Model* model, Transform global_transform)
{
	this->models.push_back({model, global_transform});
}

void RenderingSystem::addLight(void* light, Transform global_transform)
{

}

void RenderingSystem::setSkybox(Skybox* box)
{
	this->skybox = box;
}

void RenderingSystem::clearScene()
{
	this->skybox = nullptr;

	this->models.clear();
}

void RenderingSystem::RenderModel(Mesh* mesh, GLuint& texture, ShaderProgram* program, Transform& transform, Camera* camera, vector2I& screen_size)
{
	Transform cameraTransform = camera->parent_node->getGlobalTransform();
	matrix4 modelMatrix = transform.getModleMatrix(cameraTransform.getPosition());
	matrix4 mvp = camera->getProjectionMatrix(screen_size.x, screen_size.y) * cameraTransform.getOriginViewMatrix() * modelMatrix;

	vector3F ambientLight = vector3F(1.0f);//TODO ambient Light

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	program->setActiveProgram();
	program->setUniform("MVP", mvp);
	program->setUniform("modelMatrix", modelMatrix);
	program->setUniform("normalMatrix", transform.getNormalMatrix());
	program->setUniform("ambientLight", ambientLight);

	mesh->draw(program);

	program->deactivateProgram();
}