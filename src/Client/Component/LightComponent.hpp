#ifndef LIGHTCOMPONENT_HPP
#define LIGHTCOMPONENT_HPP

#include "Common/Component/Component.hpp"
#include "Client/Rendering/Lights.hpp"

class LightComponent : public Component
{
public:
	LightComponent(LightType type, BaseLight* light);

	virtual void addToWorld(World* world);
	virtual void removeFromWorld(World* world);

	LightType getLightType();
	BaseLight* getLight();

private:
	LightType lightType;
	BaseLight* light = nullptr;
};

#endif //LIGHTCOMPONENT_HPP