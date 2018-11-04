#ifndef GAMESTATE_SINGLEPLAYER_HPP
#define GAMESTATE_SINGLEPLAYER_HPP

#include "Client/GameState/GameState.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/NodeEntity.hpp"
#include "Client/Rendering/Camera.hpp"

#include "Client/PlayerController.hpp"

#include "Common/AI/QuaternionPidController.hpp"

class GameState_Singleplayer : public GameState
{
public:
	GameState_Singleplayer();
	virtual ~GameState_Singleplayer();
	virtual void update(Client* client, double delta_time);

	PlayerController player_controller;
protected:

	World* world = nullptr;

	Entity* character = nullptr;
	NodeEntity* ship = nullptr;
	
	NodeEntity* ai_ship = nullptr;
	QuaternionPidController* pid = nullptr;
};

#endif //GAMESTATE_SINGLEPLAYER_HPP