#version 140
#extension GL_ARB_explicit_attrib_location : enable

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float leafScale;
uniform float fallTime;
uniform float fallSpeed;

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex;

layout(location = 3) in vec4 in_modelRow0;
layout(location = 4) in vec4 in_modelRow1;
layout(location = 5) in vec4 in_modelRow2;
layout(location = 6) in float in_branchDepth;

out vec3 v_normal;
out vec2 v_tex;
out vec3 v_wsPos;
out vec3 v_viewDir;

///////////////////////////////////////////////////////////////
// Clip plane begin
///////////////////////////////////////////////////////////////
uniform vec4 clipPlane;
out float v_clipDistance;

void ClipPlane(vec4 vertex)
{
	// If we were emulating the FFP this should be the view space vertex
	// however we are defining the clip plane in world space
	v_clipDistance = dot(vertex, clipPlane);
}
///////////////////////////////////////////////////////////////
// Clip plane end
///////////////////////////////////////////////////////////////


void main()
{
	mat4 modelinstance = mat4(in_modelRow0.x, in_modelRow1.x, in_modelRow2.x, 0,
								in_modelRow0.y, in_modelRow1.y, in_modelRow2.y, 0,
								in_modelRow0.z, in_modelRow1.z, in_modelRow2.z, 0,
								in_modelRow0.w, in_modelRow1.w, in_modelRow2.w, 1);

	float t = max(-abs(cos(gl_InstanceID * 3213.354)) * 16 + fallTime, 0);
	vec2 xyRot = vec2(gl_InstanceID + t * 0.1,
							t * 0.2 + float(gl_InstanceID));

	mat3 rotationY = mat3(cos(xyRot.y), 0, -sin(xyRot.y), 0, 1, 0, sin(xyRot.y), 0, cos(xyRot.y));
	mat3 rotationX = mat3(1, 0, 0, 0, cos(xyRot.x), sin(xyRot.x), 0, -sin(xyRot.x), cos(xyRot.x));
	
	vec3 offset = rotationY * vec3(min(0.35*t * cos(gl_InstanceID * 1646.46), 4), 
								-t * fallSpeed * (abs(cos(gl_InstanceID * 2007.466874)) * 0.5 + 0.5), 0);

	vec4 p = model * modelinstance * vec4(0, 0, 0, 1);
	float shrink =  pow(clamp(abs(p.y) - 1.0 + offset.y, 0, 1), 4);

	vec3 pos = in_vertex * leafScale * shrink;
	vec4 wsPos = modelinstance * vec4(rotationX * pos,1.0);
	wsPos.xyz += offset;
	wsPos = model * wsPos;

	ClipPlane(wsPos);

	gl_Position = projection * view * wsPos;
	v_wsPos = wsPos.xyz;

	v_viewDir.xyz = (transpose(view) * vec4(0, 0, -1, 0)).xyz;

	v_normal =  (model * modelinstance * vec4(rotationY * rotationX * in_normal, 0)).xyz;

	v_tex = in_tex;
}