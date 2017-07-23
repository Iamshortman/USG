#version 130

#include "res/shaders/Lighting.ls"

in vec3 out_Normal;
in vec2 out_TexCoord;

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
	//fragmentColor = vec4(abs(out_Normal), 1.0f);
	fragmentColor = texture(texture1, out_TexCoord);
}
