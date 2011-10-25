#version 130

in vec3 in_vertex;

out vec3 msVert;

void main()
{
	msVert = in_vertex;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(in_vertex,1);
}
