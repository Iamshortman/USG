#version 330 core
layout (location = 0) in vec3 in_Position;

uniform mat4 LightMatrix;
uniform mat4 ModelMatrix;

out vec3 out_FragPos;

void main(void) 
{	
	out_FragPos = (ModelMatrix * vec4(in_Position, 1.0f)).xyz;
	gl_Position = LightMatrix * (ModelMatrix * vec4(in_Position, 1.0f));
}
