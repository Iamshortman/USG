#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

//Prototype class
class Client;

class GameState
{
public:
	virtual ~GameState() {};
	virtual void update(Client* client, double delta_time) = 0;
};

#endif //GAMESTATE_HPP