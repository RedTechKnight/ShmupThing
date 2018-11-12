#version 400

uniform vec3 gOffset;
uniform viewParams
{
	mat4 gView;
	mat4 gProjection;
};

layout(location = 0) in vec3 gVertexPos;
layout(location = 1) in vec3 gVertexNormal;

out Vertex
{
	vec3 position;
	vec3 normal;
};
void main()
{
		normal = gVertexNormal;
		gl_Position =  gProjection * gView * vec4(gVertexPos+gOffset,1);
		position = gl_Position.xyz;
}