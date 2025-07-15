#version 330 core
in vec4 exColor;

out vec4 outColor;
// texture samplers

void main()
{
  
  // linearly interpolate between both textures (80% container, 20% awesomeface)
  outColor = exColor;
}
