#version 130

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in ivec3 in_boneIndices;
in vec3 in_weights;

out vec3 out_Normal;
out vec2 out_TexCoord;
out vec3 out_worldPos;

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix; 
uniform mat4 localOffset = mat4(1.0); 

const int MAX_JOINTS = 50;//max joints allowed in a skeleton
const int MAX_WEIGHTS = 3;//max number of joints that can affect a vertex
uniform mat4 boneTransforms[MAX_JOINTS];
 
void main(void) 
{		
	vec4 position = vec4(in_Position, 1.0f);
	
	vec4 totalLocalPos = vec4(0.0f);
	vec4 totalNormal = vec4(0.0f);
	
	for(int i = 0; i < MAX_WEIGHTS; i++)
	{
		mat4 boneTransform = boneTransforms[in_boneIndices[i]];
		vec4 posePosition = boneTransform * position;
		totalLocalPos += posePosition * in_weights[i];
		
		vec4 worldNormal = boneTransform * vec4(in_Normal, 0.0f);
		totalNormal += worldNormal * in_weights[i];
	}
			
	if(in_weights.x == 0.0f && in_weights.y == 0.0f && in_weights.z == 0.0f)
	{
		totalLocalPos = position;
		totalNormal = vec4(in_Normal, 1.0f);		
	}
	
	gl_Position = MVP * (localOffset *  totalLocalPos);
	out_Normal = normalMatrix * totalNormal.xyz;
	out_TexCoord = in_TexCoord;
}
