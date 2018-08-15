#version 330 core
layout (location = 0) in vec3 in_Position;

uniform mat4 LightMatrix;
uniform mat4 ModelMatrix;

void main(void) 
{	
	gl_Position = LightMatrix * (ModelMatrix * vec4(in_Position, 1.0f));
}
