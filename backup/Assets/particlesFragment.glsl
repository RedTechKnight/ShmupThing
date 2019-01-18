#version 400
uniform particle
{
	vec4 offset[4096];
};

uniform sampler2D uTexture;

in vec3 tex;
out vec4 fragment;
void main()
{
	vec4 decal = texture(uTexture,tex.xy);
	int index = int(tex.z);
	
	if(decal.w > .1)
		fragment = vec4(decal.xyz,offset[index].w);
	else
		discard;
	
}