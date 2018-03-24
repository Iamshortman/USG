#version 130

#include "res/shaders/Lighting.ls"

in vec3 out_Normal;
in vec2 out_TexCoord;
in vec3 out_worldPos;
in vec4 out_lightPos;

uniform vec3 ambientLight = vec3(1.0f);

uniform sampler2D texture1;
uniform sampler2D shadow_map;

uniform DirectionalLight directinal_light;
uniform PointLight point_light;
uniform SpotLight spot_light;
uniform int lightType = 0;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

void main(void) 
{
	vec4 result = vec4(0.0);

	if(lightType == 0)
	{
		result = CalcDirectionalLight(directinal_light, out_Normal, out_worldPos);
	}
	else if(lightType == 1)
	{
		result = CalcPointLight(point_light, out_Normal, out_worldPos);
	}
	else if(lightType == 2)
	{
		result = CalcSpotLight(spot_light, out_Normal, out_worldPos);
	}
	
	vec4 trueColor = texture(texture1, out_TexCoord);
	fragmentColor = trueColor * result * CalcShadow(shadow_map, out_lightPos);
}
