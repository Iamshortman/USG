#ifndef GAMESTATE_SINGLEPLAYER_HPP
#define GAMESTATE_SINGLEPLAYER_HPP

#include "Client/GameState/GameState.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Entity/NodeEntity.hpp"
#include "Client/Rendering/Camera.hpp"

class GameState_Singleplayer : public GameState
{
public:
	GameState_Singleplayer();
	virtual ~GameState_Singleplayer();
	virtual void update(Client* client, double delta_time);

protected:
	World* world;

	Camera* camera = nullptr;

	//Entity* camera;
	NodeEntity* character;
};

#endif //GAMESTATE_SINGLEPLAYER_HPP