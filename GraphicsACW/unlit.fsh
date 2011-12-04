#version 130

uniform sampler2D diffuseMap; 

in float v_clipDistance;

out vec4 f_color;

void main()
{
	if (v_clipDistance < 0) // We are emulating gl_ClipDistance[] behaviour as it seems to be
		discard;			// poorly supported

	f_color = vec4(1);
}