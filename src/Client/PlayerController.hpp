#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP

#include "Client/Rendering/Camera.hpp"
#include "Common/Entity/Entity.hpp"

#include <stack>

class PlayerController
{
public:
	void setPlayerEntity(Entity* entity);
	void setTempPlayerEntity(Entity* entity);

	Camera* getCamera();
	World* getWorld();

	void applyUserInput(double delta_time);

private:
	Entity* getCurrentEntity();

	Entity* player_entity = nullptr;
	Entity* player_temp_entity = nullptr;
};

#endif //PLAYER_CONTROLLER_HPP