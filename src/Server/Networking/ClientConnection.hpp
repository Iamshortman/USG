#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include "Common/Types.hpp"
#include "Common/Entity/Entity.hpp"

#include <set>
#include <stack>

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

	std::stack<World*> worldsToLoad;
	std::stack<Entity*> entitiesToLoad;

	std::set<World*> worldsInFocus;
	std::set<Entity*> entitiesInFocus;

	std::stack<World*> worldsToUnload;
	std::stack<Entity*> entitiesToUnload;

private:
	//string email = ""; //TODO email based login
	string username = "";

	Entity* controllingEntity = nullptr;
	bool shoudlCloseConnection = false;
};

#endif //CLIENTCONNECTION_HPP