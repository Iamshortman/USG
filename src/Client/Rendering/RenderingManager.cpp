#include "RenderingManager.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"

#include <stack>
#include <set>

RenderingManager::RenderingManager(Window* window)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_2D);

	this->window = window;

	int windowWidth, windowHeight;
	this->window->getWindowSize(windowWidth, windowHeight);
	this->g_buffer = new G_Buffer(windowWidth, windowHeight, false);
	this->ms_g_buffer = new G_Buffer(windowWidth, windowHeight, true, 8);

	vector<TexturedVertex> vertices;
	vertices.push_back({ vector3F(-1.0f,  1.0f, 0.5f), vector3F(0.0f),  vector2F(0.0f, 1.0f) });
	vertices.push_back({ vector3F(-1.0f, -1.0f, 0.5f), vector3F(0.0f),  vector2F(0.0f, 0.0f) });
	vertices.push_back({ vector3F( 1.0f,  1.0f, 0.5f), vector3F(0.0f),  vector2F(1.0f, 1.0f) });
	vertices.push_back({ vector3F( 1.0f, -1.0f, 0.5f), vector3F(0.0f),  vector2F(1.0f, 0.0f) });

	vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	this->full_screen_quad = new TexturedMesh(vertices, indices);
	this->full_screen_quad_program = new ShaderProgram("res/shaders/ScreenTexture.vs", "res/shaders/ScreenTexture.fs");

	ShaderPool::getInstance()->getUsing("res/shaders/Textured");
}

RenderingManager::~RenderingManager()
{
	delete this->g_buffer;
	delete this->ms_g_buffer;
	delete this->full_screen_quad;
	delete this->full_screen_quad_program;
}

void RenderingManager::renderScene()
{
	int windowWidth, windowHeight, bufferWidth, bufferHeight;
	this->window->getWindowSize(windowWidth, windowHeight);
	this->g_buffer->getBufferSize(bufferWidth, bufferHeight);
	if ((windowWidth != bufferWidth) || (windowHeight != bufferHeight))
	{
		//Window Size changed, rebuild Gbuffer
		delete this->g_buffer;
		delete this->ms_g_buffer;
		this->g_buffer = new G_Buffer(windowWidth, windowHeight, false);
		this->ms_g_buffer = new G_Buffer(windowWidth, windowHeight, true, 8);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, this->ms_g_buffer->getFBO());
	this->ms_g_buffer->clearBuffer();

	//RENDER MODELS HERE!!!!!!!!!!!!!!!!!!!!!!!!

	for (auto model_list : this->models)
	{
		while (!model_list.second.empty())
		{
			auto model = model_list.second.front();

			this->RenderModel(model.first, &this->camera, model.second, this->camera_transform);

			model_list.second.pop();
		}
	}

	this->models.clear();

	this->current_texture = "";

	this->g_buffer->clearBuffer();
	//Blit all 3 color attachments + depth
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->ms_g_buffer->getFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->g_buffer->getFBO());

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(
		0, 0, windowWidth, windowHeight,
		0, 0, windowWidth, windowHeight,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(
		0, 0, windowWidth, windowHeight,
		0, 0, windowWidth, windowHeight,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glBlitFramebuffer(
		0, 0, windowWidth, windowHeight,
		0, 0, windowWidth, windowHeight,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBlitFramebuffer(
		0, 0, windowWidth, windowHeight,
		0, 0, windowWidth, windowHeight,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	this->window->clearBuffer();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->g_buffer->getPositionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->g_buffer->getNormalTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->g_buffer->getAlbedoTexture());

	//CALC LIGHTING HERE


	this->full_screen_quad_program->setActiveProgram();
	this->full_screen_quad_program->setUniform("gPosition", 0);
	this->full_screen_quad_program->setUniform("gNormal", 1);
	this->full_screen_quad_program->setUniform("gAlbedoSpec", 2);

	this->full_screen_quad->draw(this->full_screen_quad_program);

	this->full_screen_quad_program->deactivateProgram();

	this->window->updateBuffer(); 
}

void RenderingManager::setCamera(Camera camera, Transform global_transform)
{
	this->camera = camera;
	this->camera_transform = global_transform;
}

void RenderingManager::addModel(Model* model, Transform global_transform, WorldId world_id)
{
	this->models[world_id].push(std::make_pair(model, global_transform));
}

void RenderingManager::addLight(BaseLight* light, Transform global_transform, WorldId world_id)
{
	//TODO
}

void RenderingManager::RenderModel(Model* model, Camera* camera, Transform& model_transform, Transform& camera_tranform)
{
	matrix4 modelMatrix = model_transform.getModleMatrix(camera_tranform.getPosition());
	matrix4 mvp = camera->getProjectionMatrix(this->window) * camera_tranform.getOriginViewMatrix() * modelMatrix;

	vector3F ambientLight = vector3F(1.0f);//TODO ambient Light

	Mesh* mesh = MeshPool::getInstance()->get(model->getMesh());

	if (model->getTexture() != this->current_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturePool::getInstance()->get(model->getTexture()));
		this->current_texture = model->getTexture();
	}


	//Ambient pass
	ShaderProgram* ambient_shader = ShaderPool::getInstance()->get(model->getGBufferShader());

	ambient_shader->setActiveProgram();
	ambient_shader->setUniform("MVP", mvp);
	ambient_shader->setUniform("modelMatrix", modelMatrix);
	ambient_shader->setUniform("normalMatrix", model_transform.getNormalMatrix());
	ambient_shader->setUniform("ambientLight", ambientLight);

	mesh->draw(ambient_shader);

	ambient_shader->deactivateProgram();
}