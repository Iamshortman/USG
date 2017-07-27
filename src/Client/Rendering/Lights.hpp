#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include "Common/GLM_Include.hpp"

enum LightType
{
	Directional,
	Point,
	Spot,
};

class BaseLight
{
public:
	BaseLight(vector3F color, float intensity);

	vector3F getColor();
	float getIntensity();
	bool getEnabled();

	void setColor(vector3F color);
	void setIntensity(float intensity);
	void setEnabled(bool enabled);
	 
private:
	vector3F m_color;
	float m_intensity;
	bool m_enabled;
};

class DirectionalLight : public BaseLight
{
public:
	DirectionalLight(vector3F direction, vector3F color, float intensity);

	vector3F getDirection();

	void setDirection(vector3F direction);

private:
	vector3F m_direction;
};

class PointLight : public BaseLight
{
public:
	PointLight(vector3D position, float range, vector3F attenuation, vector3F color, float intensity);

	vector3D getPosition();
	float getRange();
	vector3F getAttenuation();

	void setPosition(vector3D position);
	void setRange(float range);
	void setAttenuation(vector3F attenuation);


private:
	vector3D m_position;
	float m_range;
	vector3F m_attenuation;
};

class SpotLight : public PointLight
{
public:
	SpotLight(vector3F direction, float cutoff, vector3D position, float range, vector3F attenuation, vector3F color, float intensity);

	vector3F getDirection();
	float getCutoff();

	void setDirection(vector3F direction);
	void setCutoff(float cutoff);

private:
	vector3F m_direction;
	float m_cutoff;
};

#endif //LIGHTS_HPP