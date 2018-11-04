#ifndef PID_CONTROLLER_HPP
#define PID_CONTROLLER_HPP

double clamp(double value, double max, double min);


class PidController
{
public:
	PidController(double proptional, double intergral, double derivative, double min, double max, double i_max);

	double calculate(double error, double delta_time);

	void clear();


private:
	double proptional_const; //Proptional Constant
	double intergral_const; //Integral Constant
	double derivative_const; //Derivative Constant

	double output_min, output_max;
	double intergral_max;

	double intergral_error = 0.0;
	double prev_error = 0.0;
};

#endif //PID_CONTROLLER_HPP