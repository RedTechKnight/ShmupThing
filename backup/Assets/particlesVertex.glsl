#version 400
uniform viewport
{
	mat4 view;
	mat4 projection;
};

uniform particle
{
	vec4 offset[4096];
};

layout(location = 0) in vec4 position;

out int index;
void main()
{
	index = gl_InstanceID;
	gl_Position = projection * view * vec4( offset[index].xyz + position.xyz,1);
}
