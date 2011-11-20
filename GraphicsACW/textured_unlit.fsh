#version 130

uniform sampler2D diffuseMap; 

in vec2 v_tex;
in float v_clipDistance;
in float v_alpha;

out vec4 f_color;

void main()
{
	if (v_clipDistance < 0) // We are emulating gl_ClipDistance[] behaviour as it seems to be
		discard;			// poorly supported

	f_color = texture(diffuseMap, vec2(1, -1) * v_tex) * v_alpha;
}