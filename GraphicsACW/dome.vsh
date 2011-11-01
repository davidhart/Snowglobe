#version 130

in vec3 in_vertex;
out vec3 v_normal;

void main(void)
{
   gl_Position = gl_ModelViewProjectionMatrix * vec4(in_vertex,1.0);

   v_normal = gl_NormalMatrix * normalize(in_vertex);
}