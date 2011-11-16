#version 130

uniform sampler2D texture; 

in vec3 v_normal;
in vec2 v_tex;
in float v_clipDistance;

out vec4 f_color;

void main()
{
	if (v_clipDistance < 0) // We are emulating gl_ClipDistance[] behaviour as it seems to be
		discard;			// poorly supported

	f_color = vec4(texture(texture, vec2(1, -1) * v_tex));
}