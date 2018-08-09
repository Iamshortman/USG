#ifndef CHARACTER_CONTROLLER_HPP
#define CHARACTER_CONTROLLER_HPP

#include "Common/Component/ComponentEntity.hpp"
#include "Common/Entity/Entity.hpp"

#include "Common/GLM_Include.hpp"

#include "Common/Entity/Node.hpp"

class CharacterController : public ComponentEntity
{
public:
	CharacterController(Entity* entity);
	virtual ~CharacterController();

	virtual void update(double delta_time);

	void setHead(Node* head);


	vector3D linear_input = vector3D(0.0);
	vector3D angular_input = vector3D(0.0);

	bool jump = false;

private:

	void checkOnGround();
	void doMovement(double delta_time);
	void doRotation(double delta_time);
	void doHeadRotation(double delta_time);

	bool on_ground = false;
	double ray_distance = 1.0;

	double head_rotation = 0.0;
	Node* head_node = nullptr;
};

#endif //CHARACTER_CONTROLLER_HPP