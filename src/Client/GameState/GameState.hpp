#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

//Prototype class
class Client;
class GameObject;
class Camera;

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
	GameObject * camera;
	GameObject* target;

	Camera* scene_camera;
	GameObject* scene_root;
};

class GameState_Multiplayer : public GameState
{
public:
	GameState_Multiplayer();
	virtual ~GameState_Multiplayer();
	virtual void update(Client* client, double delta_time);

private:
};

#endif //GAMESTATE_HPP