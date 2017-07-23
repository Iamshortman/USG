#include "Client/Rendering/Lights.hpp"

BaseLight::BaseLight(vector3F color, float intensity)
{
	this->setColor(color);
	this->setIntensity(intensity);
	this->setEnabled(true);
}

vector3F BaseLight::getColor()
{ 
	return this->m_color;
}

float BaseLight::getIntensity()
{ 
	return this->m_intensity;
}

bool BaseLight::getEnabled()
{
	return this->m_enabled;
}

void BaseLight::setColor(vector3F color)
{ 
	this->m_color = color;
}

void BaseLight::setIntensity(float intensity)
{ 
	this->m_intensity = intensity;
}

void BaseLight::setEnabled(bool enabled)
{
	this->m_enabled = enabled;
}

DirectionalLight::DirectionalLight(vector3F direction, vector3F color, float intensity)
	:BaseLight(color, intensity)
{
	this->setDirection(direction);
}

vector3F DirectionalLight::getDirection()
{ 
	return m_direction; 
}

void DirectionalLight::setDirection(vector3F direction)
{
	m_direction = direction;
}

PointLight::PointLight(vector3D position, float range, vector3F attenuation, vector3F color, float intensity)
	:BaseLight(color, intensity)
{
	this->setPosition(position);
	this->setRange(range);
	this->setAttenuation(attenuation);
}

vector3D PointLight::getPosition()
{
	return this->m_position;
}

float PointLight::getRange()
{
	return this->m_range;
}

vector3F PointLight::getAttenuation()
{
	return this->m_attenuation;
}

void PointLight::setPosition(vector3D position)
{
	this->m_position = position;
}

void PointLight::setRange(float range)
{
	this->m_range = range;
}

void PointLight::setAttenuation(vector3F attenuation)
{
	this->m_attenuation = attenuation;
}

SpotLight::SpotLight(vector3F direction, float cutoff, vector3D position, float range, vector3F attenuation, vector3F color, float intensity)
	:PointLight(position, range, attenuation, color, intensity)
{
	this->setDirection(direction);
}

vector3F SpotLight::getDirection()
{
	return this->m_direction;
}

float SpotLight::getCutoff()
{
	return this->m_cutoff;
}

void SpotLight::setDirection(vector3F direction)
{
	this->m_direction = direction;
}

void SpotLight::setCutoff(float cutoff)
{
	this->m_cutoff = cutoff;
}
