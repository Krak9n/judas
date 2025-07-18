#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

  // was model *
  FragPos = vec3(view * vec4(aPos, 1.0));
  Normal = aNormal;  
    
  // was projection * view
  gl_Position = projection * model * vec4(FragPos, 1.0);
}
