#include "Client/Client.hpp"

//Singletons
#include "Client/Resource/TexturePool.hpp"
#include "Client/Resource/ShaderPool.hpp"
#include "Client/Resource/MeshPool.hpp"

#include "Client/Input/InputManager.hpp"

#include "Common/GameObjectManager.hpp"

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//Create Singletons
	TexturePool* texturePool = new TexturePool();
	ShaderPool* shaderPool = new ShaderPool();
	MeshPool* meshPool = new MeshPool();

	InputManager* inputManager = new InputManager();

	GameObjectManager* gameObjectManager = new GameObjectManager();

	Client* game = new Client();

	inputManager->centerMouse();

	double deltaTime = 0.;

	Uint32 lastTime = SDL_GetTicks();
	Uint32 lastFrameTime = SDL_GetTicks();
	Uint32 currentTime = SDL_GetTicks();
	Uint32 frames = 0;

	bool mouseCaptured = false;

	while (!game->getShouldClose())
	{
		//FPS counter stuff
		currentTime = SDL_GetTicks();
		frames++;
		if (currentTime - lastTime > 1000)
		{
			game->window->setTitleString("USG FPS: " + std::to_string(frames));
			lastTime = currentTime;
			frames = 0;
		}
		Uint32 delta = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		deltaTime = ((double)delta) / 1000.0;

		game->update(deltaTime);
	}

	delete game;

	//Requires the enter button to be hit before we end.
	cin.get();

	return 0;
}