/**
 * prototype_vertex_shader.glsl
 * Chris Vig (chris@invictus.so)
 */

#version 330 core

in vec3 vertex_position;
in vec4 vertex_color;

out vec4 frag_color;

void main(void)
{
  gl_Position = vec4(vertex_position, 1.0);
  frag_color = vertex_color;
}
