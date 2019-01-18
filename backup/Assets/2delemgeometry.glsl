#version 400

uniform ImageRect
{
	vec4 rect;
	vec4 area;
};

layout(points) in;
layout(triangle_strip,max_vertices = 6) out;

out vec2 TexCoord;
void main()
{	
	gl_Position = vec4(-1 + rect.x*2,1 - rect.y*2,0,1);
	TexCoord = vec2(area.x,area.y);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect.x+rect.z)*2,1 - rect.y*2,0,1);
	TexCoord = vec2(area.x+area.z,area.y);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect.x+rect.z)*2,1 - (rect.y+rect.w)*2,0,1);
	TexCoord = vec2(area.x+area.z,area.y+area.w);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = vec4(-1 + rect.x*2,1 - rect.y*2,0,1);
	TexCoord = vec2(area.x,area.y);
	EmitVertex();
	
	gl_Position = vec4(-1 + rect.x*2,1 - (rect.y+rect.w)*2,0,1);
	TexCoord = vec2(area.x,area.y+area.w);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect.x+rect.z)*2,1 - (rect.y+rect.w)*2,0,1);
	TexCoord = vec2(area.x+area.z,area.y+area.w);
	EmitVertex();
	
	EndPrimitive();
}