#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

//#include "Common/World/World.hpp"
//#include "Client/Networking/ClientNetworkManager.hpp"
//#include "Client/Entity/PlayerController.hpp"

//Prototype class
class Client;
class GameObject;

class GameState
{
public:
	virtual ~GameState() {};
	virtual void update(Client* client, double delta_time) = 0;
};

class GameState_MainMenu : public GameState
{
public:
	GameState_MainMenu();
	virtual ~GameState_MainMenu();
	virtual void update(Client* client, double delta_time);
};

class GameState_Singleplayer : public GameState
{
public:
	GameState_Singleplayer();
	virtual ~GameState_Singleplayer();
	virtual void update(Client* client, double delta_time);

private:
	GameObject* scene_root;
};

class GameState_Multiplayer : public GameState
{
public:
	GameState_Multiplayer();
	virtual ~GameState_Multiplayer();
	virtual void update(Client* client, double delta_time);

	//PlayerController playerInterface;

private:
	//World* mainWorld = nullptr;
	//ClientNetworkManager* networkManager = nullptr;
};

#endif //GAMESTATE_HPP