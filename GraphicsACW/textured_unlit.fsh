#version 130

uniform sampler2D texture; 

in vec3 v_normal;
in vec2 v_tex;
out vec4 f_color;

void main()
{
	f_color = vec4(texture(texture, vec2(1, -1) * v_tex));
}