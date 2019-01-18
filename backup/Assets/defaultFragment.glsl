#version 400

uniform sampler2D gTexture;
out vec4 fragment;
in vec3 gColour;
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

uniform Light
{
	vec4 position[64];
	vec4 colour[64];
}light;

in Vertex
{
	vec3 position;
	vec3 normal;
	flat int index;
};

vec4 lightFragment()
{
	int numLights = int(light.colour[0].w);
	float lightFactor = 0.0f;
	vec4 finalColour = vec4(0,0,0,1);
	for(int i = 0;i < numLights;i++)
	{
		vec4 lightPos = projection * view * vec4(light.position[i].xyz,1);
		vec3 hitDirection = lightPos.xyz - position;
		float distance = length(hitDirection);
		if(distance < light.position[i].w)
		{
			hitDirection = hitDirection/distance;
			float hitAngle = dot(hitDirection,normal);
			if(hitAngle > 0)
			{
				lightFactor = hitAngle * (light.position[i].w - distance)/(light.position[i].w);
				finalColour.xyz = finalColour.xyz + light.colour[i].xyz * lightFactor;
			}
		}
	}
	return finalColour;
}

void main()
{
	vec4 lightFactor = lightFragment();
	fragment = hitflash[index] + lightFactor * .5;
}
