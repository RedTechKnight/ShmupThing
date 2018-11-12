#version 400

uniform Rects
{
	vec4 rect[20];
	vec4 area[20];
	vec4 depth[5];
};

layout(points) in;
layout(triangle_strip,max_vertices = 6) out;

out vec3 TexCoord;
void main()
{	
	int index = int(gl_in[0].gl_Position.x);
	float z = depth[index/5][index%4];
	
	gl_Position = vec4(-1 + rect[index].x*2,1 - rect[index].y*2,z,1);
	TexCoord = vec3(area[index].x,area[index].y+area[index].w,index);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect[index].x+rect[index].z)*2,1 - rect[index].y*2,z,1);
	TexCoord = vec3(area[index].x+area[index].z,area[index].y+area[index].w,index);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect[index].x+rect[index].z)*2,1 - (rect[index].y+rect[index].w)*2,z,1);
	TexCoord = vec3(area[index].x+area[index].z,area[index].y,index);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = vec4(-1 + rect[index].x*2,1 - rect[index].y*2,z,1);
	TexCoord = vec3(area[index].x,area[index].y+area[index].w,index);
	EmitVertex();
	
	gl_Position = vec4(-1 + rect[index].x*2,1 - (rect[index].y+rect[index].w)*2,z,1);
	TexCoord = vec3(area[index].x,area[index].y,index);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect[index].x+rect[index].z)*2,1 - (rect[index].y+rect[index].w)*2,z,1);
	TexCoord = vec3(area[index].x+area[index].z,area[index].y,index);
	EmitVertex();
	
	EndPrimitive();
}