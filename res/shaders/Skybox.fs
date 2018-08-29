#version 330 core
layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 2) out vec4 gPosition;
in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    gAlbedoSpec = texture(skybox, TexCoords);
	gPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}