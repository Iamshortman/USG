#ifndef GAMESTATE_EDITOR_HPP
#define GAMESTATE_EDITOR_HPP

#include "Client/GameState/GameState.hpp"
#include "Common/GLM_Include.hpp"

#include "Client/Ship/ShipTemplate.hpp"

class GameState_Editor : public GameState
{
public:
	GameState_Editor();
	virtual ~GameState_Editor();
	virtual void update(Client* client, double delta_time);

private:
	double cube_size = 3.0;

	GameObject* scene_camera;
	GameObject* scene_root;

	GameObject* cursor;
	vector3I cursor_position;

	ShipTemplate* ship_template = nullptr;
};

#endif //GAMESTATE_EDITOR_HPP