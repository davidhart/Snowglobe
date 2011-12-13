#version 140

uniform vec4 clipPlane;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float endTime;
uniform float particleSystemShape;
uniform float particleSystemHeight;
uniform float particleSpread;
uniform float particleSpeed;
uniform vec3 windDirection;
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

	float id = gl_InstanceID / 201.0;
	float ft = id + particleSpeed * time - 1;
	float deathTime = id + particleSpeed * endTime - 1;
	float t = fract(ft);
	float spread = pow(t, particleSystemShape);

	vec3 pos = vec3(sin(62.0 * id),
					particleSystemHeight * t,
					cos(163.0 * id));
	pos.xz = normalize(pos.xz) * particleSpread * cos(1231.0 * id) * spread;
	pos.xyz += pow(windDirection, vec3(t));

	vec4 vertex = (model * vec4(pos, 1.0));

	float s = 1;
	if (ft < 0) s = 0;
	if (ft - deathTime > 1 - fract(deathTime)) s = 0;

	vertex.xyz += transpose(mat3(view)) * (particleSize * s * (t*0.6 + 0.4) * in_vertex.zyx);

	gl_Position = (projection * view) * vertex;

	v_tex = in_tex;
	v_alpha = smoothstep(0.0, 0.4, t) * (1 - smoothstep(0.6, 1.0, t)) * 0.8;
}