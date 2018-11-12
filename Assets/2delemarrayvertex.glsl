#version 400

layout(location = 0) in float inVertexPos;

void main()
{
	gl_Position = vec4(gl_InstanceID,0,0,1);
}