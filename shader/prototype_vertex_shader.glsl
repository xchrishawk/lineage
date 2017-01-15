/**
 * prototype_vertex_shader.glsl
 * Chris Vig (chris@invictus.so)
 */

#version 330 core

attribute vec4 vertex_position;

void main(void)
{
  gl_Position = vertex_position;
}
