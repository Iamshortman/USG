#version 330 core
#include "res/shaders/Lighting.ls"

layout (location = 0) out vec4 fragmentColor;

in vec2 out_TexCoord;

uniform sampler2D gAlbedoSpec;
uniform sampler2D gNormal;
uniform sampler2D gPosition;

uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;

uniform SpotLight spot_light;

void main(void) 
{	
	vec3 position = texture(gPosition, out_TexCoord).xyz;
	float glow = texture(gPosition, out_TexCoord).w;
	vec3 normal = texture(gNormal, out_TexCoord).xyz;
	vec4 color = texture(gAlbedoSpec, out_TexCoord);
		
	vec4 world_space = shadowMatrix * vec4(position, glow);
	vec3 projCoords = ((world_space.xyz / world_space.w) * 0.5) + 0.5;
	
	//float depth_value = texture2D(shadowMap, projCoords.xy).r;
	fragmentColor = vec4(1.0f) * CalcShadow(shadowMap, world_space);	
}
