#version 130

const float alpha = 0.9;
const vec4 rim_col = vec4(1, 1, 1, 0.5);
const float rim_pwr = 1.2;
const float spec_pwr = 50;
const vec4 spec_col = vec4(1, 1, 1, 0.8);
const vec3 light_dir = vec3(0.7, 0.7, 0);

in vec3 v_normal;

void main(void)
{
   vec3 normal = normalize(-v_normal);
   
   vec3 reflVec =  reflect(vec3(0, 0, 1), normal);
   
   vec3 lightDir = normalize(gl_NormalMatrix * light_dir);
   
   float spec = pow(max(dot(reflVec, lightDir), 0), spec_pwr);
   
   float rim = pow(1 - max(dot(vec3(0, 0, 1), normal), 0), rim_pwr);
   
   gl_FragColor = vec4 (rim_col.rgb * rim + spec * spec_col.rgb, alpha * (rim_col.a * rim + spec));  
}