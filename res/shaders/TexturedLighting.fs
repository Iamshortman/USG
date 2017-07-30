#version 130

#include "res/shaders/Lighting.ls"

in vec3 out_Normal;
in vec2 out_TexCoord;
in vec3 out_worldPos;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform vec3 ambientLight = vec3(1.0f);
uniform sampler2D texture1;

uniform DirectionalLight directinalLights[8];
uniform int directinalCount = 0;

uniform PointLight pointLights[8];
uniform int pointCount = 0;

uniform SpotLight spotLights[8];
uniform int spotCount = 0;

void main(void) 
{
	vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	for(int i = 0; i < directinalCount; i++)
	{
		result += CalcDirectionalLight(directinalLights[i], out_Normal, out_worldPos);
	}
	
	for(int i = 0; i < pointCount; i++)
	{
		result += CalcPointLight(pointLights[i], out_Normal, out_worldPos);
	}
	
	for(int i = 0; i < spotCount; i++)
	{
		result += CalcSpotLight(spotLights[i], out_Normal, out_worldPos);
	}
	
	vec4 trueColor = texture(texture1, out_TexCoord);
	fragmentColor = trueColor * result;
}
