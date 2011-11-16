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
	// If we were emulating the FFP this should be the view space vertex
	// however we are defining the clip plane in world space
	//gl_ClipVertex = model * vec4(in_vertex, 1.0);
	gl_ClipDistance[1] = (view * model * vec4(in_vertex, 1)).x;

	mat4 modelview = view * model;
	gl_Position = (projection * view * model) * vec4(in_vertex,1.0);

	v_normal = (modelview * vec4(in_normal, 0)).xyz;
	v_tex = in_tex;
}