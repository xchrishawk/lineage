/**
 * default_fragment_shader.glsl
 * Chris Vig (chris@invictus.so)
 */

#version 330 core

/* -- Inputs -- */

in VertexToFragmentInterface
{
  vec3 vertex_normal;
  vec4 vertex_color;
} inblock;

/* -- Outputs -- */

out vec4 fragment_color;

/* -- Procedures -- */

void main(void)
{
  fragment_color = inblock.vertex_color;
}
