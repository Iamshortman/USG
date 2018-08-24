#version 330 core

layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gPosition;

in vec3 out_Normal;
in vec2 out_TexCoord;
in vec3 out_FragPos;

uniform sampler2D texture1;

void main(void) 
{
	gAlbedoSpec = texture(texture1, out_TexCoord);
	gNormal = normalize(out_Normal);
	gPosition = vec4(out_FragPos, 1.0);
}
