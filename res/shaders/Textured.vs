#version 330 core
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoord;

out vec3 out_Normal;
out vec2 out_TexCoord;
out vec3 out_FragPos;

uniform mat4 MVP;
uniform mat3 normalMatrix;  
 
void main(void) 
{	
	vec4 fragPos = MVP * vec4(in_Position, 1.0f);
	out_FragPos = fragPos.xyz;
	gl_Position = fragPos;
	out_Normal = normalMatrix * in_Normal;
	out_TexCoord = in_TexCoord;
}
