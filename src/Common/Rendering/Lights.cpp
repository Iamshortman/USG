#include "Lights.hpp"

BaseLight::BaseLight(vector3F color, float intensity)
{
	this->setColor(color);
	this->setIntensity(intensity);
	this->setEnabled(true);
}

vector3F BaseLight::getColor()
{ 
	return this->color;
}

float BaseLight::getIntensity()
{ 
	return this->intensity;
}

bool BaseLight::getEnabled()
{
	return this->enabled;
}

void BaseLight::setColor(vector3F color)
{ 
	this->color = color;
}

void BaseLight::setIntensity(float intensity)
{ 
	this->intensity = intensity;
}

void BaseLight::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

DirectionalLight::DirectionalLight(vector3F direction, vector3F color, float intensity)
	:BaseLight(color, intensity)
{
	this->setDirection(direction);
}

vector3F DirectionalLight::getDirection()
{ 
	return direction; 
}

void DirectionalLight::setDirection(vector3F direction)
{
	direction = direction;
}

PointLight::PointLight(float range, vector3F attenuation, vector3F color, float intensity)
	:BaseLight(color, intensity)
{
	this->setRange(range);
	this->setAttenuation(attenuation);
}

float PointLight::getRange()
{
	return this->range;
}

vector3F PointLight::getAttenuation()
{
	return this->attenuation;
}

void PointLight::setRange(float range)
{
	this->range = range;
}

void PointLight::setAttenuation(vector3F attenuation)
{
	this->attenuation = attenuation;
}

SpotLight::SpotLight(vector3F direction, float cutoff, float range, vector3F attenuation, vector3F color, float intensity)
	:PointLight(range, attenuation, color, intensity)
{
	this->setDirection(direction);
	this->setCutoff(cutoff);
}

vector3F SpotLight::getDirection()
{
	return this->direction;
}

float SpotLight::getCutoff()
{
	return this->cutoff;
}

void SpotLight::setDirection(vector3F direction)
{
	this->direction = direction;
}

void SpotLight::setCutoff(float cutoff)
{
	this->cutoff = cutoff;
}
