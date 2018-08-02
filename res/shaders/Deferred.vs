#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 2) in vec2 in_TexCoord;

out vec2 out_TexCoord;
 
void main(void) 
{	
	gl_Position = vec4(in_Position, 1.0f);
	out_TexCoord = in_TexCoord;
}
