#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Common/World/WorldSolarSystem.hpp"
#include "Client/Networking/ClientNetworkManager.hpp"

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
	WorldSolarSystem* mainWorld = nullptr;
};

class GameState_Multiplayer : public GameState
{
public:
	GameState_Multiplayer();
	virtual ~GameState_Multiplayer();
	virtual void update(Client* client, double deltaTime);

private:
	WorldSolarSystem* mainWorld = nullptr;
	ClientNetworkManager* networkManager = nullptr;
};

#endif //GAMESTATE_HPP