#version 330 core
#include "res/shaders/Lighting.ls"

layout (location = 0) out vec4 fragmentColor;

in vec2 out_TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform DirectionalLight directional_light;

void main(void) 
{	
	vec3 position = texture(gPosition, out_TexCoord).xyz;
	vec3 normal = texture(gNormal, out_TexCoord).xyz;
	vec4 color = texture(gAlbedoSpec, out_TexCoord);
	
	fragmentColor = color * CalcDirectionalLight(directional_light, normal, position);
}
