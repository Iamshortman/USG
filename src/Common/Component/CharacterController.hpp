#ifndef CHARACTER_CONTROLLER_HPP
#define CHARACTER_CONTROLLER_HPP

#include "Common/Component/ComponentEntity.hpp"
#include "Common/Entity/Entity.hpp"

#include "Common/GLM_Include.hpp"

class CharacterController : public ComponentEntity
{
public:
	CharacterController(Entity* entity);
	virtual ~CharacterController();

	virtual void update(double delta_time);

	vector3D linear_input = vector3D(0.0);
	bool jump = false;

private:

	bool checkOnGround();



	bool on_ground = false;
	double ray_distance = 1.0;

	double move_force = 0.8;
	double inair_move_force = 0.02;
	double brake_force = 0.2;
	double jump_force = 7.0f;

	/*
	const float MOVE_FORCE = 0.8f;
	const float INAIR_MOVE_FORCE = 0.02f;
	const float BRAKE_FORCE = 0.2f;
	const float JUMP_FORCE = 7.0f;
	const float YAW_SENSITIVITY = 0.1f;
	const float INAIR_THRESHOLD_TIME = 0.1f;
	*/
};

#endif //CHARACTER_CONTROLLER_HPP