#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include "Common/Types.hpp"
#include "Common/Entity/Entity.hpp"

#include <set>

class ClientConnection
{
public:
	ClientConnection(string username);
	~ClientConnection();

	void setControllingEntity(Entity* entity);

	string getUsername();
	Entity* getControllingEntity();

	void closeConnection();
	bool shouldCloseConnection();

	void updateEntitiesInFocus();

	std::set<Entity*> entitiesInFocus;
	std::set<EntityId> entitiesLoaded;

private:
	//string email = ""; //TODO email based login
	string username = "";

	Entity* controllingEntity = nullptr;
	bool shoudlCloseConnection = false;
};

#endif //CLIENTCONNECTION_HPP