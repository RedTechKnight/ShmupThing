#version 400
uniform TextParamList
{
	vec4 rect[20];
	vec4 textureScale[10];
	vec4 depth[5];
	vec4 colour[20];
};

uniform sampler2DArray uTextArray;
in vec3 TexCoord;
out vec4 fragment;
void main()
{
	float alpha = texture(uTextArray,TexCoord).a;
	int index = int(TexCoord.z);
	
		fragment = vec4(colour[index].xyz,alpha);
}