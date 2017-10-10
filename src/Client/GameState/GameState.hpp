#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Common/World/World.hpp"
#include "Client/Networking/ClientNetworkManager.hpp"
#include "Client/Entity/PlayerController.hpp"

//Prototype class
class Client;

class GameState
{
public:
	virtual ~GameState() {};
	virtual void update(Client* client, double deltaTime) = 0;
};

class GameState_MainMenu : public GameState
{
public:
	GameState_MainMenu();
	virtual ~GameState_MainMenu();
	virtual void update(Client* client, double deltaTime);
};

class GameState_Singleplayer : public GameState
{
public:
	GameState_Singleplayer();
	virtual ~GameState_Singleplayer();
	virtual void update(Client* client, double deltaTime);

private:
	World* mainWorld = nullptr;
	PlayerController playerInterface;
};

class GameState_Multiplayer : public GameState
{
public:
	GameState_Multiplayer();
	virtual ~GameState_Multiplayer();
	virtual void update(Client* client, double deltaTime);

	PlayerController playerInterface;

private:
	World* mainWorld = nullptr;
	ClientNetworkManager* networkManager = nullptr;
};

#endif //GAMESTATE_HPP