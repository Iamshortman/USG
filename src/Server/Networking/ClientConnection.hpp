#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include "Common/Types.hpp"
#include "Common/Entity/Entity.hpp"

#include "RakPeerInterface.h"

class ClientConnection
{
public:
	ClientConnection(RakNet::SystemAddress address);
	~ClientConnection();

	void setUsername(string username);
	void setControllingEntity(Entity* entity);

	string getUsername();
	Entity* getControllingEntity();

	void CloseConnection();
	bool shouldCloseConnection();

private:
	RakNet::SystemAddress address;
	string username = "";
	Entity* controllingEntity = nullptr;
	bool closeConnection = false;
};

#endif //CLIENTCONNECTION_HPP