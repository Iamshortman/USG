#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 out_Normal;
in vec2 out_TexCoord;
in vec3 out_FragPos;

uniform vec3 ambientLight = vec3(1.0f);
uniform sampler2D texture1;

void main(void) 
{
	gPosition = out_FragPos;
	gNormal = normalize(out_Normal);
	gAlbedoSpec = texture(texture1, out_TexCoord) * vec4(ambientLight, 1.0f);
}
