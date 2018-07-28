#include "RenderingManager.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"

#include "Common/World/World.hpp"

#include "Common/Entity/NodeEntity.hpp"

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

	this->skybox = new Skybox("res/textures/Skybox/space", "res/shaders/Skybox");
}

RenderingManager::~RenderingManager()
{
	delete this->g_buffer;
	delete this->ms_g_buffer;
	delete this->full_screen_quad;
	delete this->full_screen_quad_program;
}

void RenderingManager::RenderWorld(World* world, Camera* camera)
{
	if (world == nullptr)
	{
		return;
	}

	auto entities = world->getEntitiesInWorld();
	for (auto it = entities->begin(); it != entities->end(); it++)
	{
		Entity* entity = *it;

		if (entity->hasSubWorld())
		{
			this->RenderWorld(entity->getSubWorld(), camera);
		}

		if (entity != nullptr)
		{
			if (entity->getType() == EntityType::ENTITY)
			{
				Model* model = entity->getNodeComponent<Model>();

				if (model != nullptr && model->isEnabled())
				{
					this->RenderModel(model, camera);
				}

			}
			else if (entity->getType() == EntityType::NODE_ENTITY)
			{
				NodeEntity* node_entity = (NodeEntity*)entity;
				//TODO: something clever

				std::stack<Node*> nodes;
				for (Node* child : node_entity->getChildNodes())
				{
					nodes.push(child);
				}

				while (!nodes.empty())
				{
					Node* node = nodes.top();
					nodes.pop();
					
					Model* model = node->getNodeComponent<Model>();

					if (model != nullptr && model->isEnabled())
					{
						this->RenderModel(model, camera);
					}

					for (Node* child : node->getChildNodes())
					{
						nodes.push(child);
					}
				}

			}
		}
	}
}

void RenderingManager::Render(World* baseWorld, Camera* camera)
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

	if (this->skybox != nullptr)
	{
		skybox->draw(camera, windowWidth, windowHeight);
		
		glClearDepth(0.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	World* world = baseWorld;

	while (world->hasParentWorld())
	{
		world = world->getParentWorld();
	}

	this->RenderWorld(world, camera);

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

	this->full_screen_quad_program->setActiveProgram();
	this->full_screen_quad_program->setUniform("gPosition", 0);
	this->full_screen_quad_program->setUniform("gNormal", 1);
	this->full_screen_quad_program->setUniform("gAlbedoSpec", 2);

	this->full_screen_quad->draw(this->full_screen_quad_program);

	this->full_screen_quad_program->deactivateProgram();

	this->window->updateBuffer();
}

void RenderingManager::RenderModel(Model* model, Camera* camera)
{
	Transform globalTransform = model->parent_node->getGlobalTransform();
	Transform cameraTransform = camera->parent_node->getGlobalTransform();
	matrix4 modelMatrix = globalTransform.getModleMatrix(cameraTransform.getPosition());
	matrix4 mvp = camera->getProjectionMatrix(this->window) * cameraTransform.getOriginViewMatrix() * modelMatrix;

	vector3F ambientLight = vector3F(1.0f);//TODO ambient Light

	Mesh* mesh = MeshPool::getInstance()->get(model->getMesh());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexturePool::getInstance()->get(model->getTexture()));

	//Ambient pass
	ShaderProgram* ambient_shader = ShaderPool::getInstance()->get(model->getAmbientShader());

	ambient_shader->setActiveProgram();
	ambient_shader->setUniform("MVP", mvp);
	ambient_shader->setUniform("modelMatrix", modelMatrix);
	ambient_shader->setUniform("normalMatrix", globalTransform.getNormalMatrix());
	ambient_shader->setUniform("ambientLight", ambientLight);

	mesh->draw(ambient_shader);

	ambient_shader->deactivateProgram();
}