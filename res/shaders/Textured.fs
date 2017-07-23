#version 130

in vec3 out_Normal;
in vec2 out_TexCoord;
in vec3 out_WorldPos;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform vec3 ambientLight = vec3(1.0f);
uniform sampler2D texture1;

void main(void) 
{
	//fragmentColor = vec4(abs(out_Normal), 1.0f);
	fragmentColor = texture(texture1, out_TexCoord) * vec4(ambientLight, 1.0f);
}
