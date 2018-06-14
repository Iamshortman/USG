#ifndef GAMESTATE_SINGLEPLAYER_HPP
#define GAMESTATE_SINGLEPLAYER_HPP

#include "Client/GameState/GameState.hpp"

#include "Common/EntityX_Include.hpp"

class GameState_Singleplayer : public GameState
{
public:
	GameState_Singleplayer();
	virtual ~GameState_Singleplayer();
	virtual void update(Client* client, double delta_time);

	EntityX ecs_system;

private:
};

#endif //GAMESTATE_SINGLEPLAYER_HPP