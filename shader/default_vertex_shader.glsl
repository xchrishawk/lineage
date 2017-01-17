/**
 * default_vertex_shader.glsl
 * Chris Vig (chris@invictus.so)
 */

#version 330 core

/* -- Inputs -- */

in vec3 vertex_position;
in vec3 vertex_normal;
in vec4 vertex_color;

/* -- Outputs -- */

out VertexToFragmentInterface
{
  vec4 vertex_color;
} outblock;

/* -- Procedures -- */

void main(void)
{
  gl_Position = vec4(vertex_position, 1.0);
  outblock.vertex_color = vertex_color;
}
