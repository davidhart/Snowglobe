#version 130

out vec4 f_color;

void main(void)
{
	f_color = vec4(1);
	gl_FragDepth = 1.0;
}