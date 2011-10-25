#version 130

in vec3 msVert;

void main()
{
	gl_FragColor = vec4(pow(abs(normalize(msVert)), vec3(2,2,2)), 1);
}
