#include "RenderingManager.hpp"

#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/TexturePool.hpp"

#include <stack>
#include <set>

RenderingManager::RenderingManager(Window* window)
{
	this->window = window;

	int windowWidth, windowHeight;
	this->window->getWindowSize(windowWidth, windowHeight);
	this->g_buffer = new G_Buffer(windowWidth, windowHeight);

	ShaderPool::instance->loadShader("Textured", "res/shaders/Textured.vs", "res/shaders/Textured.fs", { { 0, "in_Position" },{ 1, "in_Normal" },{ 2, "in_TexCoord" } });
	ShaderPool::instance->loadShader("Textured_Lighting", "res/shaders/Textured.vs", "res/shaders/Textured_Lighting.fs", { { 0, "in_Position" },{ 1, "in_Normal" },{ 2, "in_TexCoord" } });
	ShaderPool::instance->loadShader("Textured_Shadow", "res/shaders/Textured_Shadow.vs", "res/shaders/Textured_Shadow.fs", { { 0, "in_Position" } });
	
	MeshPool::instance->loadMesh("res/models/SmallCube.obj");
	MeshPool::instance->loadMesh("res/models/CubeShip1.obj");
	MeshPool::instance->loadMesh("res/models/Player_Capsule.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Hull.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Engine.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Wing.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Canopy_Outside.obj");
	MeshPool::instance->loadMesh("res/models/Cobra/Cockpit.obj");
	MeshPool::instance->loadMesh("res/models/plane.obj");

	TexturePool::instance->loadTexture("res/textures/1K_Grid.png");


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

RenderingManager::~RenderingManager()
{

}

void RenderingManager::renderScene(GameObject* scene_root, Camera* camera)
{
	int windowWidth, windowHeight, bufferWidth, bufferHeight;
	this->window->getWindowSize(windowWidth, windowHeight);
	this->g_buffer->getBufferSize(bufferWidth, bufferHeight);
	if ((windowWidth != bufferWidth) || (windowHeight != bufferHeight))
	{
		//Window Size changed, rebuild Gbuffer
		delete this->g_buffer;
		this->g_buffer = new G_Buffer(windowWidth, windowHeight);
	}

	//discovery mode
	std::stack<GameObject*> nodes;
	nodes.push(scene_root);

	std::set<ComponentModel*> models;
	std::set<void*> lights;
	std::set<void*> models_transparent;
	
	while (!nodes.empty())
	{
		GameObject* node = nodes.top();
		nodes.pop();


		if (node->hasComponent<Component>())//TODO Distance Culling
		{

		}

		if (node->hasComponent<ComponentModel>())
		{
			ComponentModel* model = node->getComponent<ComponentModel>();
			if (model->isEnabled())
			{
				models.insert(model);
			}
		}

		for (GameObject* child : node->children)
		{
			nodes.push(child);
		}
	}


	glBindFramebuffer(GL_FRAMEBUFFER, this->g_buffer->getFBO());
	this->g_buffer->clearBuffer();

	for (ComponentModel* model : models)
	{
		this->RenderModel(model, camera);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	this->window->clearBuffer();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->g_buffer->getFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // default FBO
	glBlitFramebuffer(
		0, 0, windowWidth, windowHeight,
		0, 0, windowWidth, windowHeight,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->window->updateBuffer();
}

void RenderingManager::RenderModel(ComponentModel* model, Camera* camera)
{
	Transform globalTransform = model->getParent()->getGlobalTransform();
	matrix4 modelMatrix = globalTransform.getModleMatrix(camera->getPosition());
	matrix4 mvp = camera->getProjectionMatrix(this->window) * camera->getOriginViewMatrix() * modelMatrix;

	vector3F ambientLight = vector3F(1.0f);//TODO ambient Light

	Mesh* mesh = MeshPool::instance->getMesh(model->getMesh());

	if (model->temp_mesh != nullptr)
	{
		mesh = (Mesh*)model->temp_mesh;
	}

	glBindTexture(GL_TEXTURE_2D, TexturePool::instance->getTexture(model->getTexture()));

	//Ambient pass
	ShaderProgram* ambient_shader = ShaderPool::instance->getShader(model->getAmbientShader());

	ambient_shader->setActiveProgram();
	ambient_shader->setUniform("MVP", mvp);
	ambient_shader->setUniform("modelMatrix", modelMatrix);
	ambient_shader->setUniform("normalMatrix", globalTransform.getNormalMatrix());
	ambient_shader->setUniform("ambientLight", ambientLight);

	mesh->draw(ambient_shader);

	ambient_shader->deactivateProgram();
}
