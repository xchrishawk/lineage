/**
 * default_fragment_shader.glsl
 * Chris Vig (chris@invictus.so)
 */

#version 330 core
#extension GL_ARB_explicit_uniform_location : require

/* -- Uniforms -- */

layout (location = 3) uniform vec4 ambient_light_color;
layout (location = 4) uniform float ambient_light_intensity;

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
  vec4 ambient_light = ambient_light_color * ambient_light_intensity;
  fragment_color = inblock.vertex_color * ambient_light;
}
