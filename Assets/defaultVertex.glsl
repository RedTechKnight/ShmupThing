#version 400

uniform viewParams
{
	mat4 view;
	mat4 projection;
};

uniform Offsets
{
	vec4 offset[256];
	vec4 hitflash[256];
};

layout(location = 0) in vec3 inVertexPos;
layout(location = 1) in vec3 inVertexNormal;

out Vertex
{
	vec3 position;
	vec3 normal;
	flat int index;
};
void main()
{
	index = gl_InstanceID;	
	normal = inVertexNormal;
	gl_Position =  projection * view * vec4(inVertexPos+offset[gl_InstanceID].xyz,1);
	position = gl_Position.xyz;
}