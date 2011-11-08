#version 130

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_vertex;
out vec3 v_normal;

void main(void)
{
	mat4 modelview = view * model;
	gl_Position = (projection * view * model) * vec4(in_vertex,1.0);

	v_normal = (modelview * vec4(in_vertex, 0)).xyz;
}