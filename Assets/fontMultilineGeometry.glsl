#version 400

uniform TextLineParam
{
	vec4 rect[16];
	vec4 textureArea[16];
}textParam;

layout(points) in;
layout(triangle_strip,max_vertices = 6) out;

out vec2 TexCoord;
void main()
{	
	int instanceID = int(gl_in[0].gl_Position.z);
	vec4 rect = textParam.rect[instanceID];
	vec4 area = textParam.textureArea[instanceID];
	gl_Position = vec4(-1 + rect.x*2,1 - rect.y*2,0,1);
	TexCoord = vec2(area.x,area.y);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect.x+rect.z)*2,1 - rect.y*2,0,1);
	TexCoord = vec2(area.z,area.y);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect.x+rect.z)*2,1 - (rect.y+rect.w)*2,0,1);
	TexCoord = vec2(area.z,area.w);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = vec4(-1 + rect.x*2,1 - rect.y*2,0,1);
	TexCoord = vec2(area.x,area.y);
	EmitVertex();
	
	gl_Position = vec4(-1 + rect.x*2,1 - (rect.y+rect.w)*2,0,1);
	TexCoord = vec2(area.x,area.w);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect.x+rect.z)*2,1 - (rect.y+rect.w)*2,0,1);
	TexCoord = vec2(area.z,area.w);
	EmitVertex();
	
	EndPrimitive();
}