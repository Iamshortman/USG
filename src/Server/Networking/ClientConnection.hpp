#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include "Common/Types.hpp"
#include "Common/Entity/Entity.hpp"
#include "Common/World/World.hpp"
#include "RakNetTypes.h"

#include <set>
#include <queue>

class ClientConnection
{
public:
	ClientConnection(RakNet::SystemAddress address);
	~ClientConnection();

	void setControllingEntity(Entity* entity);

	string getUsername();
	RakNet::SystemAddress getAddress();
	Entity* getControllingEntity();

	void closeConnection();
	bool shouldCloseConnection();

	void updateEntitiesInFocus();

	std::queue<WorldId> worldsToLoad;
	std::queue<EntityId> entitiesToLoad;

	std::set<WorldId> worldsInFocus;
	std::set<EntityId> entitiesInFocus;

	std::queue<WorldId> worldsToUnload;
	std::queue<EntityId> entitiesToUnload;

private:
	RakNet::SystemAddress address;

	Entity* controllingEntity = nullptr;
	bool shoudlCloseConnection = false;
};

#endif //CLIENTCONNECTION_HPP