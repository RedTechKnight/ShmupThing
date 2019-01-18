#version 400

uniform TextParamList
{
	vec4 rect[20];
	vec4 textureScale[10];
	vec4 depth[5];
	vec4 colour[20];
};

layout(points) in;
layout(triangle_strip,max_vertices = 6) out;

out vec3 TexCoord;
void main()
{	
	int index = int(gl_in[0].gl_Position.x);
	float z = depth[index/5][index%4];
	float scaleX = textureScale[index/2][(index%2)*2];
	float scaleY = textureScale[index/2][(index%2)*2+1];
	gl_Position = vec4(-1 + rect[index].x*2,1 - rect[index].y*2,z,1);
	TexCoord = vec3(0,0,index);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect[index].x+rect[index].z)*2,1 - rect[index].y*2,z,1);
	TexCoord = vec3(1*scaleX,0,index);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect[index].x+rect[index].z)*2,1 - (rect[index].y+rect[index].w)*2,z,1);
	TexCoord = vec3(1*scaleX,1*scaleY,index);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = vec4(-1 + rect[index].x*2,1 - rect[index].y*2,z,1);
	TexCoord = vec3(0,0,index);
	EmitVertex();
	
	gl_Position = vec4(-1 + rect[index].x*2,1 - (rect[index].y+rect[index].w)*2,z,1);
	TexCoord = vec3(0,1*scaleY,index);
	EmitVertex();
	
	gl_Position = vec4(-1 + (rect[index].x+rect[index].z)*2,1 - (rect[index].y+rect[index].w)*2,z,1);
	TexCoord = vec3(1*scaleX,1*scaleY,index);
	EmitVertex();
	
	EndPrimitive();
}