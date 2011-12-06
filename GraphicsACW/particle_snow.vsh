#version 140

uniform vec4 clipPlane;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float particleSpread;
uniform float particleSpeed;
uniform float particleSize;

in vec3 in_vertex;
in vec3 in_normal;
in vec2 in_tex;

out vec2 v_tex;
out float v_clipDistance;
out float v_alpha;

void main()
{
	// If we were emulating the FFP this should be the view space vertex
	// however we are defining the clip plane in world space
	v_clipDistance = dot(model * vec4(in_vertex, 1.0), clipPlane);

	float id = gl_InstanceID / 5000.0;

	vec3 pos = vec3(sin(62.0 * id),
					abs(cos(3541 * id)) * 0.5 + 0.5,
					cos(163.0 * id));
	pos.xyz = normalize(pos.xyz) * particleSpread;

	float h = pos.y + 1.5;

	float t = fract(cos(id * 5464.54) + time / h);
	pos.y -= t * h;

	vec4 vertex = (model * vec4(pos, 1.0));

	vertex.xyz += transpose(mat3(view)) * (particleSize * in_vertex.zyx);

	gl_Position = (projection * view) * vertex;

	v_tex = in_tex;
	v_alpha = smoothstep(0.0, 0.5 / h, t) * (1 - smoothstep(0.8 * h, h, t * h));
}