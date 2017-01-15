/**
 * prototype_fragment_shader.glsl
 * Chris Vig (chris@invictus.so)
 */

#version 330 core

in vec4 frag_color;

out vec4 color;

void main(void)
{
  color = frag_color;
}
