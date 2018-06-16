#ifndef GAMESTATE_SINGLEPLAYER_HPP
#define GAMESTATE_SINGLEPLAYER_HPP

#include "Client/GameState/GameState.hpp"

#include "Common/Entity/Entity.hpp"

class GameState_Singleplayer : public GameState
{
public:
	GameState_Singleplayer();
	virtual ~GameState_Singleplayer();
	virtual void update(Client* client, double delta_time);

protected:
	World* world;

	Entity* camera;
	Entity* ship;
};

#endif //GAMESTATE_SINGLEPLAYER_HPP