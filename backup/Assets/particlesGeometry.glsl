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

layout(points) in;
layout(triangle_strip,max_vertices = 6) out;

in int index[];
out vec3 tex;

void main()
{	
		vec3 right = vec3(view[0][0],view[1][0],view[2][0]);
		vec3 up = vec3(view[0][1],view[1][1],view[2][1]);
		
		
		gl_Position = gl_in[0].gl_Position + vec4(-right,0) + vec4(up,0);
		tex = vec3(0,0,index[0]);
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(right,0) + vec4(-up,0);
		tex = vec3(1,1,index[0]);
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(right,0) + vec4(up,0);
		tex = vec3(1,0,index[0]);
		EmitVertex();
		EndPrimitive();
		
		
		
		gl_Position = gl_in[0].gl_Position + vec4(right,0) + vec4(-up,0);
		tex = vec3(1,1,index[0]);
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-right,0) + vec4(-up,0);
		tex = vec3(0,1,index[0]);
		EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-right,0) + vec4(up,0);
		tex = vec3(0,0,index[0]);
		EmitVertex();
		EndPrimitive();
}