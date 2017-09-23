#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include "Common/Types.hpp"
#include "Common/Entity/Entity.hpp"

class ClientConnection
{
public:
	ClientConnection(string username);
	~ClientConnection();

	void setControllingEntity(Entity* entity);

	string getUsername();
	Entity* getControllingEntity();

	void CloseConnection();
	bool shouldCloseConnection();

private:
	//string email = ""; TODO email based login
	string username = "";

	Entity* controllingEntity = nullptr;
	bool closeConnection = false;
};

#endif //CLIENTCONNECTION_HPP