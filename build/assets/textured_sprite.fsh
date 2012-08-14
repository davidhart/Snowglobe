#version 130

uniform sampler2D diffuseMap; 

in vec2 v_tex;

out vec4 f_color;

void main()
{
	f_color = texture(diffuseMap, v_tex);
}