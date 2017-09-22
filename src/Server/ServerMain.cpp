#include "Server/Server.hpp"
#include <chrono>

int main()
{
	Server* server = new Server();

	std::chrono::high_resolution_clock::time_point last_time, current_time;
	std::chrono::duration<double> elapsed_seconds;
	last_time = current_time = std::chrono::high_resolution_clock::now();

	while (!server->getShouldClose())
	{
		//FPS counter stuff
		current_time = std::chrono::high_resolution_clock::now();
		elapsed_seconds = current_time - last_time;
		last_time = current_time;

		server->update(elapsed_seconds.count());
	}

	return 0;
}