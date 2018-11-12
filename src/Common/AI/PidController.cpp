#include "PidController.hpp"

double clamp(double value, double max, double min)
{
	if (value > max)
	{
		return max;
	}
	else if (value < min)
	{
		return min;
	}

	return value;
}

PidController::PidController(double proptional, double intergral, double derivative, double min, double max, double i_max)
{
	this->proptional_const = proptional;
	this->intergral_const = intergral;
	this->derivative_const = derivative;

	this->output_min = min;
	this->output_max = max;
	this->intergral_max = i_max;
}

double PidController::calculate(double error, double delta_time)
{
	this->intergral_error += (error * delta_time);
	this->intergral_error = clamp(this->intergral_error, this->intergral_max, -this->intergral_max);

	double derivative_error = (this->prev_error - error) * delta_time;

	double output = (this->proptional_const * error) + (this->intergral_const * this->intergral_error * delta_time) + (this->derivative_const * derivative_error);

	this->prev_error = error;
	return output;//clamp(output, this->output_max, this->output_min);
}

void PidController::clear()
{
	this->intergral_error = 0.0;
	this->prev_error = 0.0;
}
