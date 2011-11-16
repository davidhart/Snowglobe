#version 130

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_vertex;
in vec3 in_normal;
in vec2 in_tex;

in vec4 in_modelRow0;
in vec4 in_modelRow1;
in vec4 in_modelRow2;

out vec3 v_normal;
out vec2 v_tex;

void main()
{
	mat4 modelinstance = mat4(in_modelRow0.x, in_modelRow1.x, in_modelRow2.x, 0,
								in_modelRow0.y, in_modelRow1.y, in_modelRow2.y, 0,
								in_modelRow0.z, in_modelRow1.z, in_modelRow2.z, 0,
								in_modelRow0.w, in_modelRow1.w, in_modelRow2.w, 1);
	
	mat4 modelview = view * model * modelinstance;
	gl_Position = (projection * modelview) * vec4(in_vertex,1.0);

	v_normal = (modelview * vec4(in_normal, 0)).xyz;
	v_tex = in_tex;
}