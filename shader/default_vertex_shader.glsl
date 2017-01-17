/**
 * default_vertex_shader.glsl
 * Chris Vig (chris@invictus.so)
 */

#version 330 core

/* -- Uniforms -- */

uniform mat4 matrix_view;
uniform mat4 matrix_proj;

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
  gl_Position = matrix_proj * matrix_view * vec4(vertex_position, 1.0);
  outblock.vertex_color = vertex_color;
}
