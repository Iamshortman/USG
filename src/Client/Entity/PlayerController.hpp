#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include "Common/Transform.hpp"
#include "Common/Entity/Entity.hpp"
#include "Client/Entity/PlayerControllerInterface.hpp"
#include "Client/Entity/EntityPlayerClient.hpp"

class PlayerController
{
public:
	PlayerController();
	~PlayerController();

	void updatePlayerInput();
	Transform getCameraTransform();

	void bindCharacter(EntityPlayerClient* character);
	void unbindCharacter();
	Entity* getBoundCharacter();

private:
	EntityPlayerClient* character = nullptr;
};

#endif //PLAYERCONTROLLER_HPP