#version 400
uniform sampler2D uTexture;
in vec2 TexCoord;
out vec4 fragment;
void main()
{
	fragment = texture(uTexture,TexCoord);
}