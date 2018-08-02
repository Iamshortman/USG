#version 330 core

layout (location = 0) out vec4 fragmentColor;

in vec2 out_TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform vec3 ambientLight = vec3(1.0f);

void main(void) 
{	
	vec4 color = texture(gAlbedoSpec, out_TexCoord);	
	if(texture(gPosition, tex_coord).xyz == vec3(0.0, 0.0, 0.0))
	{
		return color;
	}
	else
	{
		return color * vec4(ambientLight, 1.0);
	}
	//fragmentColor = texture(gPosition, out_TexCoord);
}
