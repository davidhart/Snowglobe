#version 130

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_vertex;
in vec3 in_normal;

out vec3 v_wsPos;
out vec3 v_viewDir;
out vec3 v_normal;

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
	mat4 modelview = view * model;

	vec4 wsPos = model * vec4(in_vertex,1.0);
	ClipPlane(wsPos);

	gl_Position = projection * view * wsPos;
	v_wsPos = wsPos.xyz;

	v_viewDir.xyz = (transpose(view) * vec4(0, 0, -1, 0)).xyz;
	v_normal = (model * vec4(in_normal, 0)).xyz;
}