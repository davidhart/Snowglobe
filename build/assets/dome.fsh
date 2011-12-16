#version 130

uniform float normalScale;
uniform vec4 rimColor;

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

vec3 GetNormal()
{
	return normalize(v_normal) * normalScale;
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

void GetDiffuseSpecular(out vec3 diffuse, out vec4 specular)
{
	diffuse = vec3(0);
	specular = vec4(0);

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

		float d = 0;
		if (length(lightDistance) > 0) d = 1;

		diffuse += lights[i].diffuse * att * max((dot(lightDistance, N)), 0.0);
		specular += vec4(lights[i].specular.rgb, d) * att * pow(clamp(dot(lightDistance, viewReflected), 0.0, 1.0), lights[i].specular.w);
	}
}
///////////////////////////////////////////////////////////////
// Non Bumpmapped lighting end
///////////////////////////////////////////////////////////////

void main(void)
{
	ClipPlane();

	vec3 diffuse;
	vec4 specular;
	GetDiffuseSpecular(diffuse, specular);
   
	float rim = pow(1 - max(dot(v_viewDir, -v_normal)-0.4, 0), rimColor.a);
	float alpha = min(1, rim + specular.a);
	f_color = vec4(rim * rimColor.rgb + specular.rgb, 1.0);
}