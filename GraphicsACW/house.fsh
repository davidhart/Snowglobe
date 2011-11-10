#version 130

in vec3 v_normal;
in vec2 v_tex;
out vec4 f_color;

void main()
{
	f_color = vec4(v_tex.x,v_tex.y,1,1);
}