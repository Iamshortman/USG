#include "Server/Server.hpp"
#include <chrono>

#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"
#include "Common/Entity/EntityCharacter.hpp"

double Cycles_Per_Second = 120.0;

int main()
{
	EntityManager* entityManager = new EntityManager();

	WorldManager* worldManager = new WorldManager();

	Server* server = new Server();

	std::chrono::high_resolution_clock::time_point last_time, current_time, last_frame_time;
	std::chrono::duration<double> elapsed_seconds;
	last_frame_time = last_time = current_time = std::chrono::high_resolution_clock::now();
	int frames = 0;

	while (!server->getShouldClose())
	{
		//FPS counter stuff
		current_time = std::chrono::high_resolution_clock::now();
		frames++;

		if (current_time - last_frame_time > std::chrono::milliseconds(1000))
		{
			//printf("frames: %d\n", frames);
			last_frame_time = current_time;
			frames = 0;
		}

		elapsed_seconds = current_time - last_time;

		if (elapsed_seconds < std::chrono::duration<double>(1.0 / Cycles_Per_Second))
		{
			std::chrono::duration<double> sleepTime = std::chrono::duration<double>(1.0 / Cycles_Per_Second) - elapsed_seconds;
			std::this_thread::sleep_for(sleepTime);

			current_time = std::chrono::high_resolution_clock::now();
			elapsed_seconds = current_time - last_time;
		}

		server->update(elapsed_seconds.count());

		last_time = current_time;
	}

	return 0;
}