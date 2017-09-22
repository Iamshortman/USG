#ifndef SERVER_HPP
#define SERVER_HPP

#include "Server/Networking/ServerNetworkManager.hpp"

class Server
{
public:
	//Instance for the Singleton design pattern;
	static Server* instance;

	Server();
	virtual ~Server();

	void update(double deltaTime);
	void exitGame();
	const bool getShouldClose();

	ServerNetworkManager* networkManager;




private:
	bool shouldClose = false;
};

#endif //SERVER_HPP
