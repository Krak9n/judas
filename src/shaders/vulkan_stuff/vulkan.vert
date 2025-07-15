#version 450

// coordinates
vec2 pos[3] = vec2[](
  vec2(0.5, 0.0),
  vec2(0.0, -0.5),
  vec2(0.5, -0.5)
);

void main()
{
  // third component is the z
  // fourth is on what everything is divided
  gl_Position = vec4(pos[gl_VertexIndex], 0.0, 1.0);
}

