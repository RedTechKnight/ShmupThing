#version 400
uniform sampler2D gText;
in vec2 TexCoord;
out vec4 fragment;
void main()
{
	fragment = texture(gText,TexCoord);
}