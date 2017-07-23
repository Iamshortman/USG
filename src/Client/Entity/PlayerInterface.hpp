#ifndef PLAYERINTERFACE_HPP
#define PLAYERINTERFACE_HPP

#include "Common/Component/Transform.hpp"
#include "Common/Entity/Entity.hpp"

class PlayerInterface
{
public:
	PlayerInterface();
	~PlayerInterface();

	void updatePlayerInput();
	Transform getCameraTransform();

	void bindCharacter(Entity* character);
	void unbindCharacter();

private:
	Entity* character = nullptr;
	Entity* debugCamera;
};

#endif //PLAYERINTERFACE_HPP