/**
 * default_vertex_shader.glsl
 * Chris Vig (chris@invictus.so)
 */

#version 330 core
#extension GL_ARB_explicit_uniform_location : require

/* -- Uniforms -- */

layout (location = 0) uniform mat4 model_matrix;
layout (location = 1) uniform mat4 view_matrix;
layout (location = 2) uniform mat4 proj_matrix;

/* -- Inputs -- */

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec4 vertex_color;

/* -- Outputs -- */

out VertexToFragmentInterface
{
  vec3 vertex_normal;
  vec4 vertex_color;
} outblock;

/* -- Procedures -- */

void main(void)
{
  // set vertex position
  gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);

  // set outputs
  outblock.vertex_normal = vertex_normal;
  outblock.vertex_color = vertex_color;
}
