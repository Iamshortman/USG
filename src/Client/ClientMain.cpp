using namespace std;

#include "Client/Client.hpp"

//Singletons
#include "Client/Resource/TexturePool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/MeshPool.hpp"

#include "Common/Entity/EntityManager.hpp"
#include "Common/World/WorldManager.hpp"
#include "Client/Input/InputManager.hpp"
#include "Client/Rendering/LightManager.hpp"

int main()
{
	//Create Singletons
	TexturePool* texturePool = new TexturePool();
	ShaderPool* shaderPool = new ShaderPool();
	MeshPool* meshPool = new MeshPool();

	EntityManager* entityManager = new EntityManager();
	WorldManager* worldManager = new WorldManager();
	InputManager* inputManager = new InputManager();
	LightManager* lightManager = new LightManager();


	Client* game = new Client();

	inputManager->centerMouse();

	double deltaTime = 0;

	Uint32 lastTime = SDL_GetTicks();
	Uint32 lastFrameTime = SDL_GetTicks();
	Uint32 currentTime = SDL_GetTicks();
	Uint32 frames = 0;
	int i = 0;

	bool mouseCaptured = false;

	while (!game->getShouldClose())
	{
		//FPS counter stuff
		currentTime = SDL_GetTicks();
		frames++;
		if (currentTime - lastTime > 1000)
		{
			game->window->setTitleString("USG FPS: " + std::to_string(frames));
			//cout << "FPS: " << frames << endl;
			lastTime = currentTime;
			frames = 0;
		}
		Uint32 delta = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		deltaTime = ((double)delta) / 1000.0;
		//cout << "Update delta: " << deltaTime << endl;

		if (deltaTime > (1.0 / 45.0))
		{
			printf("Error: frame took too long: %f seconds\n", deltaTime);
		}

		game->update(deltaTime);
	}

	delete game;

	//Requires the enter button to be hit before we end.
	cin.get();

	return 0;
}