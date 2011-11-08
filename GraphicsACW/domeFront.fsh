#version 130

const float alpha = 1.0;
const vec4 rim_col = vec4(1, 1, 1, 0.8);
const float rim_pwr = 1;
const float spec_pwr = 200;
const vec4 spec_col = vec4(1, 1, 1, 0.05);
const vec3 light_dir = vec3(0.7, -0.3, 0);

in vec3 v_normal;
out vec4 f_color;

uniform mat4 model;
uniform mat4 view;

void main()
{
   vec3 normal = normalize(v_normal);
   
   vec3 reflVec =  reflect(vec3(0, 0, 1), normal);
   
   vec3 lightDir = normalize(view * model * vec4(light_dir,0)).xyz;
   
   float spec = pow(max(dot(reflVec, lightDir), 0), spec_pwr);
   
   float rim = pow(1 - max(dot(vec3(0, 0, 1), normal), 0), rim_pwr);
   
   f_color = vec4 (rim_col.rgb * rim + spec * spec_col.rgb, alpha * (rim_col.a * rim + spec));  
}