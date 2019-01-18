#version 400

layout(location = 0) in vec2 inVertexPos;

void main()
{
	gl_Position = vec4(inVertexPos.x,inVertexPos.y,gl_InstanceID,1);
}