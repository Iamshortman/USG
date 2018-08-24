#version 330 core

layout (location = 0) out vec4 fragmentColor;

in vec2 out_TexCoord;

uniform sampler2D gAlbedoSpec;
uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform vec3 ambientLight = vec3(1.0f);

void main(void) 
{	
	vec4 color = texture(gAlbedoSpec, out_TexCoord);	
	float glow = texture(gPosition, out_TexCoord).w;
	vec3 real_ambient = ambientLight + ((vec3(1.0f) - ambientLight) * glow);
	
	fragmentColor = color * vec4(real_ambient, 1.0);
}
