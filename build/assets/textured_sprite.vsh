#version 130

uniform mat4 projection;

in vec3 in_vertex;
in vec2 in_tex;

out vec2 v_tex;

void main()
{

	gl_Position = (projection) * vec4(in_vertex,1.0);

	v_tex = in_tex;
}