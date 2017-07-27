#include "Client/Client.hpp"

#include "Client/Input/InputManager.hpp"
#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"
#include "Client/Rendering/LightManager.hpp"

#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/MeshPool.hpp"
#include "Client/Resource/TexturedMesh.hpp"

#include "Client/Component/ComponentModel.hpp"

#include "Client/Resource/Skeleton.hpp"

#include "Common/Physics/CollisionShapes/BoxShape.hpp"
#include "Common/Physics/CollisionShapes/ConcaveMeshShape.hpp"

#include "Client/Component/DebugCameraComponent.hpp"

Client* Client::instance = nullptr;

Client::Client()
{
	Client::instance = this;

	this->window = new Window(800, 450, "USG");
	this->window->set3dRendering();

	this->renderingManager = new RenderingManager();
	this->renderingManager->setWindow(this->window);

	//Load Test Shaders
	ShaderPool::instance->loadShader("Textured", "res/shaders/Textured.vs", "res/shaders/Textured.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });
	//ShaderPool::instance->loadShader("TexturedAnimated", "res/shaders/TexturedAnimated.vs", "res/shaders/Textured.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });
	ShaderPool::instance->loadShader("TexturedLighting", "res/shaders/Textured.vs", "res/shaders/TexturedLighting.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });
	
	MeshPool::instance->loadModel("Ship", "res/models/LargeBlockShip.obj", true);
	MeshPool::instance->loadModel("SmallCube", "res/models/Cube.obj", true);
	MeshPool::instance->loadModel("TestChar", "res/models/TestChar/TestChar.dae", false);

	TexturePool::instance->loadTexture("res/textures/1K_Grid.png");
	TexturePool::instance->loadTexture("res/models/TestChar/diffuse.png");

	this->tempWorld = WorldManager::instance->createNewWorld();
	//this->tempWorld->setGravity(vector3D(0.0, -9.8, 0.0));

	this->tempCamera = new Camera();
	this->tempCamera->setProjection(45.0f, 0.1f, 100.0f);

	Entity* cube = EntityManager::instance->createNewEntity();
	cube->createRigidBody();
	//cube->addToWorld(this->tempWorld);
	//cube->setTransform(Transform(vector3D(0.0, 0.0, 20.0), quaternionD(0.924, 0.0, 0.383, 0.0)));

	cube->getRigidBody()->addChildShape(new BoxShape(vector3D(2.0)), Transform());

	cube->getRigidBody()->setDampening(0.0, 0.0);
	cube->getRigidBody()->applyTorqueImpulse(cube->getTransform().getLeft() * 10.0);

	//cube->getRigidBody()->setLinearVelocity(vector3D(0.0, 0.0, 20.0));

	ComponentModel* model = new ComponentModel();
	cube->addComponent("model", model);
	model->model.setMesh("SmallCube");
	model->model.setShader("Textured");
	model->model.setLightingShader("TexturedLighting");
	model->model.addTexture("res/textures/1K_Grid.png", 0);

	Entity* ship = EntityManager::instance->createNewEntity();
	ship->createRigidBody();
	ship->addToWorld(this->tempWorld);
	ship->setTransform(Transform(vector3D(0.0, 0.0, 20.0), quaternionD(0.707, 0.0, 0.707, 0.0)));

	ship->getRigidBody()->addChildShape(new BoxShape(vector3D(30.0, 16.0, 100.0)), Transform());

	ship->getRigidBody()->setMass(1000.0);
	ship->getRigidBody()->setDampening(0.0, 0.0);

	ship->setSubWorld(WorldManager::instance->createNewWorld());

	ComponentModel* model1 = new ComponentModel();
	ship->addComponent("model", model1);
	model1->model.setMesh("Ship");
	model1->model.setShader("Textured");
	model1->model.setLightingShader("TexturedLighting");
	model1->model.addTexture("res/textures/1K_Grid.png", 0);

	LightManager::instance->addDirectionalLight(ship->getSubWorld()->worldId, new DirectionalLight(vector3F(0.0f, -1.0f, 0.0f), vector3F(4.0f, 0.0f, 6.0f), 0.4f));
	
	ship->getSubWorld()->setGravity(vector3D(0.0, -9.8, 0.0));

	Entity* actor = EntityManager::instance->createNewEntity();
	actor->addToWorld(ship->getSubWorld());
	actor->setTransform(Transform(vector3D(-24.0, -1.0, 0.0)));
	actor->createRigidBody();

	actor->getRigidBody()->addChildShape(new BoxShape(vector3D(0.5, 1.0, 0.5)), Transform(vector3D(0.0, 1.0, 0.0)));
	actor->getRigidBody()->setMass(1.0);

	ComponentModel* model2 = new ComponentModel();
	actor->addComponent("model", model2);
	model2->model.setMesh("TestChar");
	model2->model.setShader("Textured");
	model2->model.setLightingShader("TexturedLighting");
	model2->model.addTexture("res/models/TestChar/diffuse.png", 0);

	//Skeleton* skeleton = Skeleton::loadSkeleton("res/models/TestChar/TestChar.dae");
	//model2->model.skeleton = skeleton;

	//Ship Inside
	Entity* interior = EntityManager::instance->createNewEntity();
	interior->addToWorld(ship->getSubWorld());
	interior->createRigidBody(0.0, new ConcaveMeshShape("res/models/LargeBlockShip.obj"));

	cube->addToWorld(ship->getSubWorld());


	//Debug Camera
	this->debugCamera = EntityManager::instance->createNewEntity();
	this->debugCamera->addToWorld(this->tempWorld);
	this->debugCamera->addComponent("DebugCamera", new DebugCameraComponent(5.0, 0.5));

}

Client::~Client()
{
	delete this->window;
	delete this->renderingManager;
}

void Client::update(double deltaTime)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			InputManager::instance->setMouseLock(!InputManager::instance->getMouseLock());
			continue;
		}


		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			exitGame();
			continue;
		}

		InputManager::instance->processEvent(event);

		this->window->HandleEvent(event);
	}

	InputManager::instance->update(deltaTime);
	WorldManager::instance->update(deltaTime);


	//Debug Camera for now
	this->tempCamera->setCameraTransform(this->debugCamera->getRenderTransform());

	this->window->clearBuffer();
	this->renderingManager->RenderWorld(this->tempWorld, this->tempCamera);
	this->window->updateBuffer();
}

void Client::exitGame()
{
	shouldClose = true;
}
 
const bool Client::getShouldClose()
{
	return shouldClose;
}
