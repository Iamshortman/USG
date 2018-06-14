#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

struct DebugCamera
{
	DebugCamera(double linear, double angular);

	//meter per second
	double linearSpeed;

	//rad per second
	double angularSpeed;

	void update(double delta_time);
};

#endif //PLAYERCONTROL_HPP
