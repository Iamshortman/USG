#ifndef PLAYERCONTROLLERINTERFACE_HPP
#define PLAYERCONTROLLERINTERFACE_HPP

#include "Common/Transform.hpp"
#include "Common/Entity/Entity.hpp"

class PlayerControllerInterface
{
public:
	virtual void updatePlayerInput() = 0;
	virtual Transform getCameraTransform() = 0;
};

#endif //PLAYERCONTROLLERINTERFACE_HPP