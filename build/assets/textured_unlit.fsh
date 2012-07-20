#version 130

uniform sampler2D diffuseMap; 

in vec2 v_tex;
in float v_alpha;

///////////////////////////////////////////////////////////////
// Clip plane begin
///////////////////////////////////////////////////////////////
in float v_clipDistance;

void ClipPlane()
{
	if (v_clipDistance < 0) // We are emulating gl_ClipDistance[] behaviour as it seems to be
		discard;			// poorly supported
}
///////////////////////////////////////////////////////////////
// Clip plane end
///////////////////////////////////////////////////////////////

out vec4 f_color;

void main()
{
	ClipPlane();

	f_color = texture(diffuseMap, v_tex) * vec4(1, 1, 1, v_alpha);
}