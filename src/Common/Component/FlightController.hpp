#ifndef FLIGHTCONTROLLER_HPP
#define FLIGHTCONTROLLER_HPP

#include "Common/GLM_Include.hpp"
#include "Common/EntityX_Include.hpp"
#include "Common/Physics/RigidBody.hpp"

class FlightController
{
public:
	vector3D linear_input = vector3D(0.0);
	vector3D angular_input = vector3D(0.0);
	bool flight_assist = false;

	vector3D thruster_torque = vector3D(400.0);
	double thruster_force[6] = { 20000.0, 20000.0, 20000.0, 20000.0, 600000.0, 30000.0 };
};

class FlightControllerSystem : public System<FlightControllerSystem>
{
public:
	void update(EntityManager& es, EventManager& events, TimeDelta delta_time) override
	{
		ComponentHandle<FlightController> flight_controller_pattern;
		ComponentHandle<RigidBody> rigid_body_pattern;
		ComponentHandle<Transform> transform_pattern;
		for (Entity entity : es.entities_with_components(flight_controller_pattern, rigid_body_pattern, transform_pattern))
		{
			ComponentHandle<FlightController> flight_controller = entity.component<FlightController>();
			ComponentHandle<RigidBody> rigid_body = entity.component<RigidBody>();
			Transform transform = Transforms::getWorldTransform(entity);

			this->UpdateAngularVelocity(delta_time, flight_controller.get(), rigid_body.get(), transform);
			this->UpdateLinearVelocity(delta_time, flight_controller.get(), rigid_body.get(), transform);

			if (rigid_body->getLinearVelocity() != vector3D(0.0) || rigid_body->getAngularVelocity() != vector3D(0.0))
			{
				rigid_body->Activate(true);
			}
		}
	};

	void UpdateLinearVelocity(double delta_time, FlightController* flight_controller, RigidBody* rigid_body, Transform& transform)
	{
		vector3D linearVelocity = rigid_body->getLinearVelocity();

		double mass = rigid_body->getMass();

		for (int i = 0; i < 3; i++)
		{
			vector3D direction = transform.getDirection(i);
			double velocity = glm::dot(direction, linearVelocity);
			size_t input_direction = (flight_controller->linear_input[i] > 0) ? (i * 2) : ((i * 2) + 1); //i*2 = positive direction (i*2)+1 = negitive direction
			double force = flight_controller->linear_input[i] * flight_controller->thruster_force[input_direction];

			linearVelocity += (direction * (force / mass)  * delta_time);
		}

		//When Flight Assist is enabled the ship will attempt to stop any movement on axes not currently being used.
		if (flight_controller->flight_assist)
		{
			for (int i = 0; i < 3; i++)
			{
				if (flight_controller->linear_input[i] == 0.0)
				{
					vector3D direction = transform.getDirection(i);
					double velocity = glm::dot(direction, linearVelocity);
					if (velocity != 0.0)
					{
						double braking_sign = (velocity > 0) ? -1.0 : 1.0;
						size_t braking_direction = (velocity > 0) ? ((i * 2) + 1) : (i * 2);
						double braking_force = flight_controller->thruster_force[braking_direction] * delta_time;
						double velocity_change = (braking_force / mass) * braking_sign;

						if (velocity > 0.0 && (velocity + velocity_change) < 0.0)
						{
							velocity_change = -velocity;
						}
						else if (velocity < 0.0 && (velocity + velocity_change) > 0.0)
						{
							velocity_change = -velocity;
						}

						linearVelocity += (direction * velocity_change);
					}
				}
			}
		}
		rigid_body->setLinearVelocity(linearVelocity);

		printf("Speed: %lf\n", glm::length(linearVelocity));
	};

	void UpdateAngularVelocity(double delta_time, FlightController* flight_controller, RigidBody* rigid_body, Transform& transform)
	{
		bool angular_flight_assist = true;

		for (int i = 0; i < 3; i++)
		{
			vector3D direction = transform.getDirection(i);
			if (i == 2)
			{
				direction *= -1.0;
			}

			if (flight_controller->angular_input[i] != 0.0)
			{
				double force = flight_controller->angular_input[i] * flight_controller->thruster_torque[i];
				rigid_body->applyTorqueImpulse(direction * force);
			}
		}

		if (angular_flight_assist)
		{
			for (int i = 0; i < 3; i++)
			{
				if (flight_controller->angular_input[i] == 0.0)
				{
					vector3D direction = transform.getDirection(i);
					if (i == 2)
					{
						direction *= -1.0;
					}

					double starting_velocity = glm::dot(direction, rigid_body->getAngularVelocity());

					if (starting_velocity != 0.0)
					{
						if (starting_velocity > 0.0)
						{
							rigid_body->applyTorqueImpulse(direction * -flight_controller->thruster_torque[i]);
							double new_velocity = glm::dot(direction, rigid_body->getAngularVelocity());

							if (new_velocity < 0.0)
							{
								rigid_body->setAngularVelocity(rigid_body->getAngularVelocity() + (-direction * new_velocity));
							}
						}
						else if (starting_velocity < 0.0)
						{
							rigid_body->applyTorqueImpulse(direction *  flight_controller->thruster_torque[i]);
							double new_velocity = glm::dot(direction, rigid_body->getAngularVelocity());

							if (new_velocity > 0.0)
							{
								rigid_body->setAngularVelocity(rigid_body->getAngularVelocity() - (direction * new_velocity));
							}
						}
					}
				}
			}
		}
	};
};


#endif // !FLIGHTCONTROLLER_HPP
