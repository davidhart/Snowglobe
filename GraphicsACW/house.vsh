#version 130

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_vertex;
in vec3 in_normal;
in vec2 in_tex;

out vec3 v_normal;
out vec2 v_tex;

void main()
{
	mat4 modelview = view * model;
	gl_Position = (projection * view * model) * vec4(in_vertex,1.0);

	v_normal = (modelview * vec4(in_normal, 0)).xyz;
	v_tex = in_tex;
}