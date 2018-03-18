#version 130

#include "res/shaders/Lighting.ls"

in vec3 out_Normal;
in vec2 out_TexCoord;
in vec3 out_worldPos;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform vec3 ambientLight = vec3(1.0f);
uniform sampler2D texture1;

uniform DirectionalLight directinal_lights[8];
uniform int directinal_count = 0;

uniform PointLight point_lights[8];
uniform int point_count = 0;

uniform SpotLight spot_lights[8];
uniform int spot_count = 0;

void main(void) 
{
	vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	for(int i = 0; i < directinal_count; i++)
	{
		result += CalcDirectionalLight(directinal_lights[i], out_Normal, out_worldPos);
	}
	
	for(int i = 0; i < point_count; i++)
	{
		result += CalcPointLight(point_lights[i], out_Normal, out_worldPos);
	}
	
	for(int i = 0; i < spot_count; i++)
	{
		result += CalcSpotLight(spot_lights[i], out_Normal, out_worldPos);
	}
	
	vec4 trueColor = texture(texture1, out_TexCoord);
	fragmentColor = trueColor * result;
}
