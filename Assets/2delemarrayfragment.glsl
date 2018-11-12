#version 400
uniform sampler2DArray uTexture;
in vec3 TexCoord;
out vec4 fragment;
void main()
{
	int index = int(TexCoord.z);
	fragment = texture(uTexture,TexCoord);
}