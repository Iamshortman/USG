#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>

#include "Server/Networking/ServerNetworkManager.hpp"
#include "Server/Networking/ClientConnection.hpp"

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
	vector<World*> rootWorlds;

	void addClient(string username);
	void removeClient(string username);

private:
	std::set<ClientConnection*> clients;

	bool shouldClose = false;
};

#endif //SERVER_HPP
