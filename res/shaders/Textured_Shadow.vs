#version 130

in vec3 in_Position;

uniform mat4 MVP;
 
void main(void) 
{	
	vec4 offsetPosition = vec4(in_Position, 1.0f);
	gl_Position = MVP * offsetPosition;
}
