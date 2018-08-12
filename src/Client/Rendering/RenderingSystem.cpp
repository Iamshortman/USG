#include "RenderingSystem.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"

#include "Client/Rendering/LightShaderUtil.hpp"

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

	this->deferred_ambient = new ShaderProgram("res/shaders/Deferred.vs", "res/shaders/Deferred.fs");
	this->deferred_light_directional = new ShaderProgram("res/shaders/Deferred.vs", "res/shaders/Deferred_Light_Directional.fs");
	this->deferred_light_point = new ShaderProgram("res/shaders/Deferred.vs", "res/shaders/Deferred_Light_Point.fs");
	this->deferred_light_spot = new ShaderProgram("res/shaders/Deferred.vs", "res/shaders/Deferred_Light_Spot.fs");
}

RenderingSystem::~RenderingSystem()
{
	delete this->full_screen_quad;
	delete this->deferred_ambient;
	delete this->deferred_light_directional;
}

void RenderingSystem::renderMS(GLuint render_target, G_Buffer* ms_g_buffer, G_Buffer* g_buffer, Camera* camera)
{
	vector2I ms_buffer_size = ms_g_buffer->getBufferSize();
	vector2I g_buffer_size = g_buffer->getBufferSize();

	this->generateGBuffer(ms_g_buffer, camera);

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

	this->drawAmbient(render_target, g_buffer, camera);
}

void RenderingSystem::render(GLuint render_target, G_Buffer* g_buffer, Camera* camera)
{
	this->generateGBuffer(g_buffer, camera);
	this->drawAmbient(render_target, g_buffer, camera);
}

void RenderingSystem::addModel(Model* model, Transform global_transform)
{
	this->models.push_back({model, global_transform});
}

void RenderingSystem::addDirectionalLight(DirectionalLight* directional)
{
	this->directional_lights.push_back(directional);
}

void RenderingSystem::addPointLight(PointLight* point, Transform global_transform)
{
	this->point_lights.push_back({ point, global_transform });
}

void RenderingSystem::addSpotLight(SpotLight* spot, Transform global_transform)
{
	this->spot_lights.push_back({ spot, global_transform });
}

void RenderingSystem::setSkybox(Skybox* box)
{
	this->skybox = box;
}

void RenderingSystem::setAmbientLight(vector3F ambient_light)
{
	this->ambient_light = ambient_light;
}

void RenderingSystem::clearScene()
{
	this->skybox = nullptr;

	this->models.clear();

	this->directional_lights.clear();
	this->point_lights.clear();
	this->spot_lights.clear();
}

void RenderingSystem::RenderModel(Mesh* mesh, GLuint& texture, ShaderProgram* program, Transform& transform, Camera* camera, vector2I& screen_size)
{
	Transform cameraTransform = camera->parent_node->getGlobalTransform();
	matrix4 modelMatrix = transform.getModleMatrix(cameraTransform.getPosition());
	matrix4 mvp = camera->getProjectionMatrix(screen_size.x, screen_size.y) * cameraTransform.getOriginViewMatrix() * modelMatrix;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	program->setActiveProgram();
	program->setUniform("MVP", mvp);
	program->setUniform("modelMatrix", modelMatrix);
	program->setUniform("normalMatrix", transform.getNormalMatrix());

	mesh->draw(program);

	program->deactivateProgram();
}

void RenderingSystem::generateGBuffer(G_Buffer* g_buffer, Camera* camera)
{
	vector2I g_buffer_size = g_buffer->getBufferSize();

	//TODO global clear buffer fuctions
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer->getFBO());
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (this->skybox != nullptr)
	{
		skybox->draw(camera, g_buffer_size.x, g_buffer_size.y);

		glClearDepth(0.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	//DRAW ALL MESHES
	for (int i = 0; i < this->models.size(); i++)
	{
		Mesh* mesh = MeshPool::getInstance()->get(this->models[i].first->getMesh());
		GLuint texture = TexturePool::getInstance()->get(this->models[i].first->getTexture());
		ShaderProgram* program = ShaderPool::getInstance()->get(this->models[i].first->getAmbientShader());

		this->RenderModel(mesh, texture, program, this->models[i].second, camera, g_buffer_size);
	}
}

void RenderingSystem::drawAmbient(GLuint render_target, G_Buffer* g_buffer, Camera* camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, render_target);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_buffer->getPositionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_buffer->getNormalTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, g_buffer->getAlbedoTexture());

	this->deferred_ambient->setActiveProgram();
	this->deferred_ambient->setUniform("gPosition", 0);
	this->deferred_ambient->setUniform("gNormal", 1);
	this->deferred_ambient->setUniform("gAlbedoSpec", 2);
	this->deferred_ambient->setUniform("ambientLight", this->ambient_light);

	this->full_screen_quad->draw(this->deferred_ambient);

	this->deferred_ambient->deactivateProgram();

	if (this->use_lighting)
	{
		this->drawLights(render_target, g_buffer, camera);
	}

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
}

void RenderingSystem::drawLights(GLuint render_target, G_Buffer* g_buffer, Camera* camera)
{
	Transform camera_transform = camera->parent_node->getGlobalTransform();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	this->deferred_light_directional->setActiveProgram();
	this->deferred_light_directional->setUniform("gPosition", 0);
	this->deferred_light_directional->setUniform("gNormal", 1);
	this->deferred_light_directional->setUniform("gAlbedoSpec", 2);
	this->deferred_light_directional->setUniform("ambientLight", this->ambient_light);
	for (int i = 0; i < this->directional_lights.size(); i++)
	{
		setDirectionalLight("directional_light", this->deferred_light_directional, directional_lights[i], camera_transform.position);
		this->full_screen_quad->draw(this->deferred_light_directional);
	}
	this->deferred_light_point->deactivateProgram();
	
	this->deferred_light_point->setActiveProgram();
	this->deferred_light_point->setUniform("gPosition", 0);
	this->deferred_light_point->setUniform("gNormal", 1);
	this->deferred_light_point->setUniform("gAlbedoSpec", 2);
	this->deferred_light_point->setUniform("ambientLight", this->ambient_light);
	for (int i = 0; i < this->point_lights.size(); i++)
	{
		setPointLight("point_light", this->deferred_light_point, this->point_lights[i].first, this->point_lights[i].second, camera_transform.position);
		this->full_screen_quad->draw(this->deferred_light_point);
	}
	this->deferred_light_point->deactivateProgram();

	this->deferred_light_directional->deactivateProgram();
	this->deferred_light_spot->setActiveProgram();
	this->deferred_light_spot->setUniform("gPosition", 0);
	this->deferred_light_spot->setUniform("gNormal", 1);
	this->deferred_light_spot->setUniform("gAlbedoSpec", 2);
	this->deferred_light_spot->setUniform("ambientLight", this->ambient_light);
	for (int i = 0; i < this->spot_lights.size(); i++)
	{
		setSpotLight("spot_light", this->deferred_light_spot, this->spot_lights[i].first, this->spot_lights[i].second, camera_transform.position);
		this->full_screen_quad->draw(this->deferred_light_spot);
	}
	this->deferred_light_spot->deactivateProgram();

	glDisable(GL_BLEND);
}
