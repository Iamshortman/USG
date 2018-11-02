#include "Client/Client.hpp"

#include "Client/Input/InputManager.hpp"

#include "Common/Entity/EntityManager.hpp"

#include "Common/Logger/Logger.hpp"

#include "Common/GLM_Include.hpp"

#include "tbb/tbb.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

const int number_of_transforms = 10000;
Transform transforms[number_of_transforms] = { Transform() };
matrix4 matrices[number_of_transforms];

int main()
{
	Logger::getInstance()->openLogFile("Log.txt");

	SDL_Init(SDL_INIT_EVERYTHING);
	Logger::getInstance()->log("Platform: %s\n", SDL_GetPlatform());
	Logger::getInstance()->log("CPU Cores: %d\n", SDL_GetCPUCount());
	Logger::getInstance()->log("Memory: %d MB\n", SDL_GetSystemRAM());


	tbb::task_scheduler_init task_schedular(tbb::task_scheduler_init::automatic);
	
	vector3D eye_pos = vector3D(1.0, 2.0, 3.0);

	Uint32 start = SDL_GetTicks();
	tbb::parallel_for(0, number_of_transforms, [=](size_t i)
		{
			matrices[i] = transforms[i].getModelMatrix(eye_pos);
		}
	);
	Uint32 end = SDL_GetTicks();
	Logger::getInstance()->log("Took %dms to process %d items\n", end - start, number_of_transforms);



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

		game->update(deltaTime);
		tbb::parallel_for(0, number_of_transforms, [=](size_t i)
		{
			matrices[i] = transforms[i].getModelMatrix(eye_pos);
		}
		);
		game->window->updateBuffer();
	}

	delete game;

	Logger::getInstance()->closeLogFile();

	//Requires the enter button to be hit before we end.
	cin.get();

	return 0;
}