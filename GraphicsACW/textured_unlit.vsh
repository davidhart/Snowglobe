#version 130

uniform vec4 clipPlane;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_vertex;
in vec3 in_normal;
in vec2 in_tex;

out vec2 v_tex;
out float v_clipDistance;
out float v_alpha;

void main()
{
	// If we were emulating the FFP this should be the view space vertex
	// however we are defining the clip plane in world space
	v_clipDistance = dot(model * vec4(in_vertex, 1.0), clipPlane);

	mat4 modelview = view * model;
	gl_Position = (projection * view * model) * vec4(in_vertex,1.0);

	v_tex = in_tex;
	v_alpha = 1.0;
}