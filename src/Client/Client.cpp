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
	ShaderPool::instance->loadShader("TexturedLighting", "res/shaders/Textured.vs", "res/shaders/TexturedLighting.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });
	
	ShaderPool::instance->loadShader("TexturedAnimated", "res/shaders/TexturedAnimated.vs", "res/shaders/Textured.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" }, { 3, "in_boneIndices" }, { 4, "in_weights" } });

	MeshPool::instance->loadModel("Ship", "res/models/LargeBlockShip.obj", true);
	MeshPool::instance->loadModel("SmallCube", "res/models/Cube.obj", true);
	//MeshPool::instance->loadModel("Ship_Inside", "res/models/Ship_Inside.obj", true);
	//MeshPool::instance->loadModel("Ship_Outside", "res/models/Ship_Outside.obj", true);

	TexturePool::instance->loadTexture("res/textures/1K_Grid.png");

	this->tempWorld = WorldManager::instance->createNewWorld();
	//this->tempWorld->setGravity(vector3D(0.0, -9.8, 0.0));

	this->tempCamera = new Camera();
	this->tempCamera->setProjection(45.0f, 0.1f, 100.0f);

	Entity* cube = EntityManager::instance->createNewEntity();
	cube->createRigidBody();
	//cube->addToWorld(this->tempWorld);
	cube->setTransform(Transform(vector3D(0.0, 0.0, -30.0)));

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

	ship->getRigidBody()->setMass(100000.0);
	ship->getRigidBody()->setDampening(0.0, 0.0);

	//ship->getRigidBody()->setLinearVelocity(vector3D(0.0, 0.0, 2.0));
	//ship->getRigidBody()->setAngularVelocity(vector3D(0.0, 1.0, 0.0));

	ship->setSubWorld(WorldManager::instance->createNewWorld());
	ComponentModel* model1 = new ComponentModel();
	//ship->addComponent("model", model1);
	model1->model.setMesh("Ship");
	model1->model.setShader("Textured");
	model1->model.setLightingShader("TexturedLighting");
	model1->model.addTexture("res/textures/1K_Grid.png", 0);
	
	ship->getSubWorld()->setGravity(vector3D(0.0, -9.8, 0.0));

	//Ship Inside
	Entity* interior = EntityManager::instance->createNewEntity();
	interior->addToWorld(ship->getSubWorld());
	interior->createRigidBody(0.0, new ConcaveMeshShape("res/models/LargeBlockShip.obj"));

	interior->addComponent("model", model1);

	cube->addToWorld(ship->getSubWorld());

	LightManager::instance->addPointLight(ship->getSubWorld()->worldId, new PointLight(vector3D(0.0), 300.0f, vector3F(0.2f, 0.001f, 0.0f), vector3F(1.0f), 0.2f));
	LightManager::instance->addPointLight(ship->getSubWorld()->worldId, new PointLight(vector3D(0.0, 0.0, 50.0), 200.0f, vector3F(0.2f, 0.01f, 0.0f), vector3F(0.01f, 0.4f, 0.7f), 0.7f));

	LightManager::instance->addDirectionalLight(this->tempWorld->worldId, new DirectionalLight(glm::normalize(vector3F(0.4f, 1.0f, -0.2f)), vector3F(1.0f), 0.4f));

	//SmallShip
	/*Entity* smallShip = EntityManager::instance->createNewEntity();
	smallShip->createRigidBody();
	smallShip->addToWorld(ship->getSubWorld());
	smallShip->getRigidBody()->addChildShape(new BoxShape(vector3D(4.5, 2.0, 7.5)), Transform(vector3D(0.0, 0.0, -1.5)));
	smallShip->getRigidBody()->addChildShape(new BoxShape(vector3D(1.5, 2.0, 1.75)), Transform(vector3D(0.0, 0.0, 6.75)));
	smallShip->getRigidBody()->setMass(1000.0);
	smallShip->getRigidBody()->setDampening(0.0, 0.0);
	smallShip->setSubWorld(WorldManager::instance->createNewWorld());

	smallShip->setTransform(Transform(vector3D(0.0, 0.0, -20.0)));

	ComponentModel* outside = new ComponentModel();
	smallShip->addComponent("model", outside);
	outside->model.setMesh("Ship_Outside");
	outside->model.setShader("Textured");
	outside->model.setLightingShader("TexturedLighting");
	outside->model.addTexture("res/textures/1K_Grid.png", 0);

	Entity* smallShipInterior = EntityManager::instance->createNewEntity();
	smallShipInterior->addToWorld(smallShip->getSubWorld());
	smallShipInterior->createRigidBody(0.0, new ConcaveMeshShape("res/models/Ship_Inside_Phys.obj"));

	ComponentModel* inside = new ComponentModel();
	smallShipInterior->addComponent("model", inside);
	inside->model.setMesh("Ship_Inside");
	inside->model.setShader("Textured");
	inside->model.setLightingShader("TexturedLighting");
	inside->model.addTexture("res/textures/1K_Grid.png", 0);*/


	if (true)
	{
		MeshPool::instance->loadModel("AnimTest", "res/models/AnimTest.dae", false);

		Entity* cube2 = EntityManager::instance->createNewEntity();
		cube2->createRigidBody(10.0, new BoxShape(vector3D(1.0)));

		ComponentModel* componetModel = new ComponentModel();
		Model* cubeModel = &componetModel->model;
		cubeModel->setMesh("AnimTest");
		cubeModel->addTexture("res/textures/1K_Grid.png", 0);

		cubeModel->setShader("TexturedAnimated");
		//cubeModel->setLightingShader("TexturedLighting");
		cubeModel->skeleton = Skeleton::loadSkeleton("res/models/AnimTest.dae");
		Skeleton::loadSkeleton("res/models/AnimTest1.dae");
		cube2->addComponent("model", componetModel);

		cube2->addToWorld(ship->getSubWorld());
	}

	if (true)
	{
		MeshPool::instance->loadModel("TestChar", "res/models/TestChar1/model.dae", false);

		Entity* testChar = EntityManager::instance->createNewEntity();
		testChar->createRigidBody(10.0, new BoxShape(vector3D(1.0)));

		ComponentModel* componetModel = new ComponentModel();
		Model* cubeModel = &componetModel->model;
		cubeModel->setMesh("TestChar");
		cubeModel->addTexture("res/textures/1K_Grid.png", 0);

		cubeModel->setShader("TexturedAnimated");
		//cubeModel->setLightingShader("TexturedLighting");
		cubeModel->skeleton = Skeleton::loadSkeleton("res/models/TestChar1/model.dae");
		testChar->addComponent("model", componetModel);

		testChar->addToWorld(ship->getSubWorld());
		testChar->setTransform(Transform(vector3D(0.0, 0.0, -10.0f)));
	}


	//Debug Camera
	this->debugCamera = EntityManager::instance->createNewEntity();
	this->debugCamera->addToWorld(this->tempWorld);
	this->debugCamera->addComponent("DebugCamera", new DebugCameraComponent(5.0, 0.5));
	//this->debugCamera->addToWorld(smallShip->getSubWorld());
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
