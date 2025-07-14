#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 inColor;

out vec4 exColor;

uniform mat4 model_plane;
uniform mat4 view_plane;
uniform mat4 projection_plane;

void main()
{
  gl_Position = projection_plane * view_plane * model_plane * vec4(aPos, 1.0f);
  exColor = inColor;
}


