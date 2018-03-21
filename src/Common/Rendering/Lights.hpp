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
	
	virtual LightType getLightType() = 0;

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

	virtual LightType getLightType() { return LightType::Directional; };

private:
	vector3F m_direction;
};

class PointLight : public BaseLight
{
public:
	PointLight(float range, vector3F attenuation, vector3F color, float intensity);

	float getRange();
	vector3F getAttenuation();

	void setRange(float range);
	void setAttenuation(vector3F attenuation);

	virtual LightType getLightType() { return LightType::Point; };

private:
	float m_range;
	vector3F m_attenuation;
};

class SpotLight : public PointLight
{
public:
	SpotLight(vector3F direction, float cutoff, float range, vector3F attenuation, vector3F color, float intensity);

	vector3F getDirection();
	float getCutoff();

	void setDirection(vector3F direction);
	void setCutoff(float cutoff);

	virtual LightType getLightType() { return LightType::Spot; };

private:
	vector3F m_direction;
	float m_cutoff;
};

#endif //LIGHTS_HPP