#version 130

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float drawDepth;

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex;

layout(location = 3) in vec4 in_modelRow0;
layout(location = 4) in vec4 in_modelRow1;
layout(location = 5) in vec4 in_modelRow2;
layout(location = 6) in float in_branchDepth;

flat out vec3 v_normal;
flat out vec2 v_tex;
flat out vec3 v_wsPos;
flat out vec3 v_viewDir;

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
	
	mat4 modelmatrix = model * modelinstance;

	float scale = 1 - clamp(in_branchDepth - (drawDepth - 1), 0, 1);

	vec3 pos = in_vertex * scale;

	vec4 wsPos = modelmatrix * vec4(pos,1.0);
	ClipPlane(wsPos);

	gl_Position = projection * view * wsPos;
	v_wsPos = wsPos.xyz;

	v_viewDir.xyz = (transpose(view) * vec4(0, 0, -1, 0)).xyz;

	v_normal = (modelmatrix * vec4(in_normal, 0)).xyz;

	v_tex = in_tex;
}