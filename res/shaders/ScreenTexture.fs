#version 330 core
layout (location = 0) out vec4 fragmentColor;

in vec2 out_TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main(void) 
{
	fragmentColor = texture(gPosition, out_TexCoord);
}
