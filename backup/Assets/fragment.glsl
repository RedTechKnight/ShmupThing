#version 400

uniform sampler2D gTexture;
out vec4 fragment;
in vec3 gColour;
uniform viewParams
{
	mat4 gView;
	mat4 gProjection;
};
uniform Light
{
	vec4 position[16];
	vec4 direction[16];
	vec4 cone[16];
}light;

uniform float hit;

in Vertex
{
	vec3 position;
	vec3 normal;
};

float lightFragment()
{
	float lightFactor = 0.0f;
	
	for(unsigned int i = 0;i < 16;i++)
	{
		if(light.position[i].w == 1)
		{
			vec4 lightPos = gProjection * gView * light.position[i];
			vec3 hitDirection = lightPos.xyz - position;
			float distance = length(hitDirection);
			if(distance < light.direction[i].x)
			{
				hitDirection = hitDirection/distance;
				float hitAngle = dot(hitDirection,normal);
				if(hitAngle > 0)
				{
					lightFactor += hitAngle * (light.direction[i].x - distance)/light.direction[i].x;
				}
			}
		}
	}
	return lightFactor;
}

void main()
{
	float lightFactor = lightFragment();
	fragment = vec4(1,0,0,0) * hit + vec4(1,1,1,1) * lightFactor;
}
