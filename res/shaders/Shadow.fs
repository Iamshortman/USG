#version 330 core

layout (location = 0) out vec4 gAlbedoSpec;

in vec3 out_FragPos;
void main(void) 
{
	gAlbedoSpec = vec4(out_FragPos, 1.0);
}
