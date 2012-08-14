#version 130

uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;

uniform vec3 matSpecular;

in vec2 v_tex;
out vec4 f_color;

///////////////////////////////////////////////////////////////
// Clip plane begin
///////////////////////////////////////////////////////////////
in float v_clipDistance;

void ClipPlane()
{
	if (v_clipDistance < 0) // We are emulating gl_ClipDistance[] behaviour as it seems to be
		discard;			// poorly supported
}
///////////////////////////////////////////////////////////////
// Clip plane end
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// Non Bumpmapped lighting begin
///////////////////////////////////////////////////////////////
#define NUM_LIGHTS 4
uniform vec3 ambient;
struct Light
{
	vec4 specular;       // {specular.rgb, specular power}
	vec3 diffuse;
	vec4 position;       // { pos.xyz, 1} for point / spotlight 
                             // or { direction.xyz, 0 } for directional light
	vec3 spotDirection;
	vec3 spot;           // { cos(innerangle), cos(outerangle), power }
                             // or { 0, 0, 0 } if point / directional light
	vec3 attenuation;    // {constant attenuation, linear attenuation, quadratic attenuation}
};
uniform Light lights[NUM_LIGHTS];

in vec3 v_wsPos;
in vec3 v_viewDir;
in vec3 v_normal;

in vec4 v_shadow;

vec3 GetNormal()
{
	return normalize(v_normal);
}

void GetLightDistanceDirection(int id, out vec3 lightDistance, out vec3 lightDir)
{   
	if (lights[id].position.w != 0.0)
		lightDistance = lights[id].position.xyz - v_wsPos.xyz;
	else
		lightDistance = normalize(-lights[id].position.xyz);
   
	lightDir = normalize(-lights[id].spotDirection);
}

vec3 GetViewDir()
{
	return normalize(v_viewDir);
}

void GetDiffuseSpecular(out vec3 diffuse, out vec3 specular)
{
	diffuse = vec3(0);
	specular = vec3(0);

	vec3 N = GetNormal();
	vec3 viewReflected = reflect(GetViewDir(), N);
   
	for (int i = 0; i < NUM_LIGHTS; ++i)
	{
		vec3 lightDistance, lightDir;
		GetLightDistanceDirection(i, lightDistance, lightDir);
      
		float lengthDistance = length(lightDistance);
		lightDistance = normalize(lightDistance);
      
		float att = clamp(1.0/(lights[i].attenuation[0] +
						  lights[i].attenuation[1]*lengthDistance +
						  lights[i].attenuation[2]*lengthDistance*lengthDistance), 0.0, 1.0);
      
		// spotlight enabled
		if (lights[i].spot[1] < 1.0)
		{
			float spot = (dot(lightDir.xyz, lightDistance) - (lights[i].spot[1])) / ((lights[i].spot[0]) - (lights[i].spot[1]));
			spot = pow(clamp(spot, 0.0, 1.0), lights[i].spot[2]);
         
			att *= spot;
			att *= spot;
		}
		diffuse += lights[i].diffuse * att * max((dot(lightDistance, N)), 0.0);
		specular += lights[i].specular.rgb * att * pow(clamp(dot(lightDistance, viewReflected), 0.0, 1.0), lights[i].specular.w);
	}
}
///////////////////////////////////////////////////////////////
// Non Bumpmapped lighting end
///////////////////////////////////////////////////////////////

float shadowLookup(vec4 coord)
{
	float shadow = 1;

	float distanceFromLight = texture(shadowMap,coord.st).z;

	if (v_shadow.w > 0.0)
	{
		if (distanceFromLight < coord.z)
		{
			shadow = 0;
		}
	}

	return shadow;
}

void main(void)
{
	ClipPlane();

	vec4 base = texture(diffuseMap, v_tex);

	vec3 diffuse, specular;
	GetDiffuseSpecular(diffuse, specular);


	vec4 shadowCoordinateWdivide = v_shadow / v_shadow.w ;
		
	// Used to lower moire pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;

	/*
	float shadow = 0;
		
	float distanceFromLight = texture(shadowMap,shadowCoordinateWdivide.st).z;
		
	if (v_shadow.w > 0.0)
	{
		if (distanceFromLight < shadowCoordinateWdivide.z)
		{
			diffuse = vec3(0);
		}
	}*/

	float shadow = 0;
	
	vec2 shadowmapInvRes = vec2(1.0/1024, 1.0/1024);

	for (int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			shadow += shadowLookup(shadowCoordinateWdivide+vec4((x-2.5)*shadowmapInvRes.x, (y-2.5)*shadowmapInvRes.y, 0, 0));
		}
	}

	shadow /= 16.0;

	f_color = vec4(min(ambient + diffuse * shadow, vec3(1)) * base.rgb + specular * matSpecular, base.a);
}