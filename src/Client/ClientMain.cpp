#include "Client/Client.hpp"

#include "Client/Input/InputManager.hpp"

#include "Common/Entity/EntityManager.hpp"

#include "Common/Logger/Logger.hpp"

int main()
{
	Logger::getInstance()->openLogFile("Log.txt");

	SDL_Init(SDL_INIT_EVERYTHING);

	Client* game = new Client();

	InputManager::getInstance()->centerMouse();

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

		if (deltaTime == 0)
		{
			deltaTime = 1.0 / 60.0;
		}

		game->update(deltaTime);
	}

	delete game;

	Logger::getInstance()->closeLogFile();

	//Requires the enter button to be hit before we end.
	cin.get();

	return 0;
}