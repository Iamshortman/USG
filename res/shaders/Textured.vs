#version 130

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord; 

out vec3 out_Normal;
out vec2 out_TexCoord;
out vec3 out_worldPos;
out vec4 out_lightPos;

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix; 
uniform mat4 localOffset = mat4(1.0); 
 
uniform mat4 lightSpaceMVP = mat4(1.0); 
 
void main(void) 
{	
	vec4 offsetPosition = localOffset * vec4(in_Position, 1.0f);
	gl_Position = MVP * offsetPosition;
	out_Normal = normalMatrix * (localOffset * vec4(in_Normal, 1.0f)).xyz;
	out_TexCoord = in_TexCoord;
	out_worldPos = (modelMatrix * offsetPosition).xyz;
	
	out_lightPos = lightSpaceMVP * offsetPosition;
}
