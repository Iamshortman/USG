#version 330 core
#include "res/shaders/Lighting.ls"

layout (location = 0) out vec4 fragmentColor;

in vec2 out_TexCoord;

uniform sampler2D gAlbedoSpec;
uniform sampler2D gNormal;
uniform sampler2D gPosition;

uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;

uniform DirectionalLight directional_light;

void main(void) 
{	
	vec3 position = texture(gPosition, out_TexCoord).xyz;
	float glow = texture(gPosition, out_TexCoord).w;
	vec3 normal = texture(gNormal, out_TexCoord).xyz;
	vec4 color = texture(gAlbedoSpec, out_TexCoord);
		
	vec4 world_space = shadowMatrix * vec4(position, 1.0);
	color = color * CalcDirectionalLight(directional_light, normal, position) * CalcShadow(shadowMap, world_space);
	color *= (1.0 - glow);
	fragmentColor = color;	
}