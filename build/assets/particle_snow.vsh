#version 140

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float endTime;
uniform float particleSpread;
uniform float particleSpeed;
uniform float particleSize;

in vec3 in_vertex;
in vec3 in_normal;
in vec2 in_tex;

out vec2 v_tex;
out float v_alpha;

///////////////////////////////////////////////////////////////
// Clip plane begin
///////////////////////////////////////////////////////////////
uniform vec4 clipPlane;
out float v_clipDistance;

void ClipPlane(vec4 vertex)
{
	// If we were emulating the FFP this should be the view space vertex
	// however we are defining the clip plane in world space
	v_clipDistance = dot(vertex, clipPlane);
}
///////////////////////////////////////////////////////////////
// Clip plane end
///////////////////////////////////////////////////////////////

void main()
{
	// If we were emulating the FFP this should be the view space vertex
	// however we are defining the clip plane in world space
	v_clipDistance = dot(model * vec4(in_vertex, 1.0), clipPlane);

	float id = gl_InstanceID / 5000.0;

	vec3 pos = vec3(fract(34234.0 * id) * 2.0 - 1.0,
					abs(cos(3541 * id)) * 0.5 + 0.5,
					fract(13413.43 * id) * 2.0 - 1.0);
	pos.xyz = normalize(pos.xyz) * particleSpread;

	float h = pos.y + 1.5;

	float timeOffset = cos(id * 5464.54);

	float deathTime = timeOffset + (endTime - h) / h;
	float ft = timeOffset + (time - h) / h;
	float t = fract(ft);

	float s = 1;
	if (ft < 0) s = 0;
	if (ft - deathTime > 1 - fract(deathTime)) s = 0;

	pos.y -= t * h;

	vec4 vertex = (model * vec4(pos, 1.0));

	vertex.xyz += transpose(mat3(view)) * (particleSize * s * in_vertex.zyx);

	ClipPlane(vertex);
	gl_Position = (projection * view) * vertex;

	v_tex = in_tex;
	v_alpha = smoothstep(0.0, 0.5 / h, t) * (1 - smoothstep(0.8 * h, h, t * h));
}