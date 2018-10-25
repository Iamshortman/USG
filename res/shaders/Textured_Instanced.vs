#version 330 core
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoord;
layout (location = 3) in mat4 in_model_matrix;

out vec3 out_Normal;
out vec2 out_TexCoord;
out vec3 out_FragPos;

uniform mat4 view_proejction_matrix;
 
void main(void) 
{	
	out_Normal = (in_model_matrix * vec4(in_Normal, 0.0f)).xyz;
	out_TexCoord = in_TexCoord;
	out_FragPos = (in_model_matrix * vec4(in_Position, 1.0f)).xyz;
	
	gl_Position = (view_proejction_matrix * in_model_matrix) * vec4(in_Position, 1.0f);
}
